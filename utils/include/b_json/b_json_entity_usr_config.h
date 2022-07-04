/*
 * 版权所有 (c) 华为技术有限公司 2022
 */

#ifndef OHOS_FILEMGMT_BACKUP_B_JSON_ENTITY_USR_CONFIG_H
#define OHOS_FILEMGMT_BACKUP_B_JSON_ENTITY_USR_CONFIG_H

#include <string>
#include <vector>

#include "b_json/b_json_cached_entity.h"
#include "json/json.h"

namespace OHOS::FileManagement::Backup {
class BJsonEntityUsrConfig {
public:
    /**
     * @brief 构造方法，具备T(Json::Value&)能力的构造函数
     *
     * @param Json对象引用
     */
    BJsonEntityUsrConfig(Json::Value &obj);

    /**
     * @brief 从JSon对象中通过includeDirs字段获取待备份目录模式串清单
     *
     * @return 待备份目录模式串清单
     * @note 如果用户没有配置该字段，则返回默认打包目录{@link BConstants::PATHES_TO_BACKUP}
     * @note 如果用户配置了空数组，则返回""表示生成空打包文件
     */
    std::vector<std::string> GetIncludeDirs() const;

    /**
     * @brief 从JSon对象中获取排除目录列表
     *
     * @return 排除目录
     */
    std::vector<std::string> GetExcludeDirs() const;

    /**
     * @brief 从JSon对象中获取备份权限
     *
     * @return 备份权限
     */
    bool GetAllowToBackup() const;

private:
    Json::Value &obj_;
};
} // namespace OHOS::FileManagement::Backup

#endif // OHOS_FILEMGMT_BACKUP_B_JSON_ENTITY_USR_CONFIG_H