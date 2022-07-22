/*
 * 版权所有 (c) 华为技术有限公司 2022
 */
#include "ext_extension_stub.h"

#include <cstdint>
#include <sstream>

#include "b_error/b_error.h"
#include "filemgmt_libhilog.h"

namespace OHOS::FileManagement::Backup {
using namespace std;
ExtExtensionStub::ExtExtensionStub()
{
    opToInterfaceMap_[CMD_GET_FILE_HANDLE] = &ExtExtensionStub::CmdGetFileHandle;
    opToInterfaceMap_[CMD_HANDLE_CLAER] = &ExtExtensionStub::CmdHandleClear;
    opToInterfaceMap_[CMD_HANDLE_BACKUP] = &ExtExtensionStub::CmdHandleBackup;
    opToInterfaceMap_[CMD_PUBLISH_FILE] = &ExtExtensionStub::CmdPublishFile;
}

int32_t ExtExtensionStub::OnRemoteRequest(uint32_t code,
                                          MessageParcel &data,
                                          MessageParcel &reply,
                                          MessageOption &option)
{
    HILOGI("Begin to call procedure indexed %{public}u", code);
    auto interfaceIndex = opToInterfaceMap_.find(code);
    if (interfaceIndex == opToInterfaceMap_.end() || !interfaceIndex->second) {
        stringstream ss;
        ss << "Cannot response request " << code << ": unknown procedure";
        return BError(BError::Codes::EXT_INVAL_ARG, ss.str());
    }

    const std::u16string descriptor = ExtExtensionStub::GetDescriptor();
    const std::u16string remoteDescriptor = data.ReadInterfaceToken();
    if (descriptor != remoteDescriptor) {
        return BError(BError::Codes::EXT_INVAL_ARG, "Invalid remote descriptor");
    }

    return (this->*(interfaceIndex->second))(data, reply);
}

ErrCode ExtExtensionStub::CmdGetFileHandle(MessageParcel &data, MessageParcel &reply)
{
    HILOGE("Begin");
    string fileName;
    if (!data.ReadString(fileName)) {
        return BError(BError::Codes::EXT_INVAL_ARG, "Failed to receive fileName").GetCode();
    }

    UniqueFd fd = GetFileHandle(fileName);
    if (!reply.WriteFileDescriptor(fd)) {
        return BError(BError::Codes::EXT_BROKEN_IPC, "Failed to send out the file").GetCode();
    }

    return BError(BError::Codes::OK);
}

ErrCode ExtExtensionStub::CmdHandleClear(MessageParcel &data, MessageParcel &reply)
{
    HILOGE("Begin");
    ErrCode res = HandleClear();
    if (!reply.WriteInt32(res)) {
        stringstream ss;
        ss << "Failed to send the result " << res;
        return BError(BError::Codes::EXT_BROKEN_IPC, ss.str()).GetCode();
    }
    return BError(BError::Codes::OK);
}

ErrCode ExtExtensionStub::CmdHandleBackup(MessageParcel &data, MessageParcel &reply)
{
    HILOGE("Begin");
    ErrCode res = HandleBackup();
    if (!reply.WriteInt32(res)) {
        stringstream ss;
        ss << "Failed to send the result " << res;
        return BError(BError::Codes::EXT_BROKEN_IPC, ss.str()).GetCode();
    }
    return BError(BError::Codes::OK);
}

ErrCode ExtExtensionStub::CmdPublishFile(MessageParcel &data, MessageParcel &reply)
{
    HILOGE("Begin");
    string fileName;
    if (!data.ReadString(fileName)) {
        return BError(BError::Codes::EXT_INVAL_ARG, "Failed to receive fileName");
    }

    ErrCode res = PublishFile(fileName);
    if (!reply.WriteInt32(res)) {
        stringstream ss;
        ss << "Failed to send the result " << res;
        return BError(BError::Codes::EXT_BROKEN_IPC, ss.str()).GetCode();
    }
    return BError(BError::Codes::OK);
}
} // namespace OHOS::FileManagement::Backup