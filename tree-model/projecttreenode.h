#ifndef PROJECTTREENODE_H
#define PROJECTTREENODE_H
#include "treenode.h"
#include <QObject>

class ProjectTreeNode : public QObject, public TreeNode
{
Q_OBJECT

#define TEXT "text"
#define ICON "icon"
#define CHILD "child"
#define WIDGET "widget"
#define TYPE "type"
#define CHILD "child"
#define ID "id"

public:
    enum TreeNodeType
    {
        //the node has child node, used for add sub node,etc
        DeviceList = 0,
        FormulaList,
        CalcVarList,
        AlarmRuleList,

        //used for del current node,etc
        DeviceNode,
        FormulaNode,
        CalcVarNode,
        AlarmRuleNode
    };

    ProjectTreeNode(QMap<QString, QString> map, ProjectTreeNode *parentItem);
    ~ProjectTreeNode();

    QString text();
    QString icon();
    bool isSelectable();

    int widgetIndex();
    int nodeType();
    int deviceId();
    int formulaId();
    int calcVarId();
    int alarmRuleId();
signals:
    void beginModifyTree();
    void endModifyTree();

private:
    QMap<QString, QString> _map;

public:
//static function
    static ProjectTreeNode *init2ProjectTreeStructure();
    static void addNewDeviceNode(int deviceId, bool bInit=false);
    static void deleteDeviceNode(int deviceId);
    static void addNewFormulaNode(int formulaId, bool bInit=false);
    static void deleteFormulaNode(int formulaId);
    static void addNewCalcVarNode(int calcVarId, bool bInit=false);
    static void deleteCalcVarNode(int calcVarId);
    static void addNewAlarmRuleNode(int alarmRuleId, bool bInit=false);
    static void deleteAlarmRuleNode(int alarmRuleId);
//static member
private:
    static ProjectTreeNode* rootNode;
    static ProjectTreeNode* deviceListNode;
    static ProjectTreeNode* formulaListNode;
    static ProjectTreeNode* calcVarListNode;
    static ProjectTreeNode* alarmRuleListNode;
    static void _traverseJsonRecursion(const QJsonArray &_jsonArray, ProjectTreeNode *parentNode);
    static void _fillListNodeFromConfig();
};

#endif // PROJECTTREENODE_H
