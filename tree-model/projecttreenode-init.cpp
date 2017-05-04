#include "projecttreenode.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include "deviceparatablemanager.h"
#include "formuladeftablemanager.h"
#include "calcvartablemanager.h"
#include "alarmruletablemanager.h"

ProjectTreeNode* ProjectTreeNode::rootNode = 0;
ProjectTreeNode* ProjectTreeNode::deviceListNode = 0;
ProjectTreeNode* ProjectTreeNode::formulaListNode = 0;
ProjectTreeNode* ProjectTreeNode::calcVarListNode = 0;
ProjectTreeNode* ProjectTreeNode::alarmRuleListNode = 0;

QJsonArray _readProjectTreeJson()
{
    QString jsonFileName = ":/json/json/tree.json";
    QFile jsonFile(jsonFileName);
    if (!jsonFile.open(QIODevice::OpenModeFlag::ReadOnly | QIODevice::OpenModeFlag::Text))
    {
        Q_UNREACHABLE();
    }

    QJsonParseError parserError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonFile.readAll(), &parserError);
    if (parserError.error != QJsonParseError::ParseError::NoError)
    {
        Q_UNREACHABLE();
    }

    Q_ASSERT(jsonDoc.isObject());
    QJsonObject jsonObject = jsonDoc.object();

    Q_ASSERT(jsonObject.contains("tree"));
    QJsonValue jsonValue = jsonObject.value("tree");

    Q_ASSERT(jsonValue.isArray());
    return jsonValue.toArray();
}

void ProjectTreeNode::_traverseJsonRecursion(const QJsonArray &_jsonArray, ProjectTreeNode *parentNode)
{
    for (int i = 0; i < _jsonArray.size(); i++)
    {
        QJsonValue jsonValue = _jsonArray.at(i);
        Q_ASSERT(jsonValue.isObject());

        QJsonObject jsonObject = jsonValue.toObject();
        QStringList keyList = jsonObject.keys();

        QMap<QString, QString> map;
        foreach (QString key, keyList)
        {
            if(key != CHILD)
            {
                map.insert(key, jsonObject.value(key).toString("N/A"));
            }
        }

        ProjectTreeNode *childNode = new ProjectTreeNode(map, parentNode);

        if(map.contains(TYPE))
        {
            int type = map.value(TYPE).toInt();
            switch (type) {
            case ProjectTreeNode::TreeNodeType::DeviceList:
                deviceListNode = childNode;
                break;
            case ProjectTreeNode::TreeNodeType::FormulaList:
                formulaListNode = childNode;
                break;
            case ProjectTreeNode::TreeNodeType::CalcVarList:
                calcVarListNode = childNode;
                break;
            case ProjectTreeNode::TreeNodeType::AlarmRuleList:
                alarmRuleListNode = childNode;
                break;
            default:
                break;
            }
        }

        if (jsonObject.contains(CHILD))
        {
            Q_ASSERT(jsonObject.value(CHILD).isArray());
            QJsonArray jsonArray = jsonObject.value(CHILD).toArray();
            _traverseJsonRecursion(jsonArray, childNode);
        }
    }
}

void ProjectTreeNode::_fillListNodeFromConfig()
{
    QList<int> deviceIdList = DeviceParaTableManager::deviceParaTableManager()->getDeviceIdList();
    foreach (int deviceId, deviceIdList)
    {
        addNewDeviceNode(deviceId, true);
    }

    QList<int> formulaIdList = FormulaDefTableManager::formulaDefinitionTableManager()->getFormulaIdList();
    foreach (int formulaId, formulaIdList)
    {
        addNewFormulaNode(formulaId, true);
    }

    QList<int> calcVarIdList = CalcVarTableManager::calcVarTableManager()->getCalVarIdList();
    foreach (int calcVarId, calcVarIdList)
    {
        addNewCalcVarNode(calcVarId, true);
    }

    QList<int> alarmRuleIdList = AlarmRuleTableManager::alarmRuleTableManager()->getAlarmRuleIdList();
    foreach (int alarmRuleId, alarmRuleIdList)
    {
        addNewAlarmRuleNode(alarmRuleId, true);
    }
}

ProjectTreeNode* ProjectTreeNode::init2ProjectTreeStructure()
{
    rootNode = new ProjectTreeNode(QMap<QString, QString>(), 0);
    QJsonArray jsonArray = _readProjectTreeJson();
    _traverseJsonRecursion(jsonArray, rootNode);
    _fillListNodeFromConfig();

    return rootNode;
}

