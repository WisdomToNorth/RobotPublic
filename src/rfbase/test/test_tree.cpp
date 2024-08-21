

#include <gtest/gtest.h>

#include "rfbase/rfidtree.h"

namespace rfbase
{

TEST(TestDataUtils, test_tree)
{
    const TreeNodeId nullptrId = 0;
    RfIdTree<std::string> tree;
    std::vector<TreeNodeId> vecTreeNodeId;
    auto fnAppendChild = [&](TreeNodeId parentId, const char *str)
    {
        const TreeNodeId id = tree.appendChild(parentId, str);
        vecTreeNodeId.push_back(id);
        return id;
    };
    const TreeNodeId n0 = fnAppendChild(nullptrId, "0");
    const TreeNodeId n0_1 = fnAppendChild(n0, "0-1");
    const TreeNodeId n0_2 = fnAppendChild(n0, "0-2");
    const TreeNodeId n0_1_1 = fnAppendChild(n0_1, "0-1-1");
    const TreeNodeId n0_1_2 = fnAppendChild(n0_1, "0-1-2");
    std::sort(vecTreeNodeId.begin(), vecTreeNodeId.end());

    EXPECT_EQ(tree.nodeParent(n0_1), n0);
    EXPECT_EQ(tree.nodeParent(n0_2), n0);
    EXPECT_EQ(tree.nodeParent(n0_1_1), n0_1);
    EXPECT_EQ(tree.nodeParent(n0_1_2), n0_1);
    EXPECT_EQ(tree.nodeChildFirst(n0_1), n0_1_1);
    EXPECT_EQ(tree.nodeChildLast(n0_1), n0_1_2);
    EXPECT_EQ(tree.nodeSiblingNext(n0_1_1), n0_1_2);
    EXPECT_EQ(tree.nodeSiblingPrevious(n0_1_2), n0_1_1);
    EXPECT_EQ(tree.nodeSiblingNext(n0_1_2), nullptrId);

    {
        std::string strPreOrder;
        traverseTree_preOrder(tree, [&](TreeNodeId id) { strPreOrder += " " + tree.nodeData(id); });
        EXPECT_EQ(strPreOrder, " 0 0-1 0-1-1 0-1-2 0-2");
    }

    {
        std::string strPostOrder;
        traverseTree_postOrder(tree,
                               [&](TreeNodeId id) { strPostOrder += " " + tree.nodeData(id); });
        EXPECT_EQ(strPostOrder, " 0-1-1 0-1-2 0-1 0-2 0");
    }

    {
        std::vector<TreeNodeId> vecTreeNodeIdVisited;
        traverseTree_unorder(tree, [&](TreeNodeId id) { vecTreeNodeIdVisited.push_back(id); });
        std::sort(vecTreeNodeIdVisited.begin(), vecTreeNodeIdVisited.end());
        EXPECT_EQ(vecTreeNodeIdVisited, vecTreeNodeId);
    }
}
} // namespace rfbase
