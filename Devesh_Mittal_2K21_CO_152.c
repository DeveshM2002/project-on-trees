#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAXLEN 100

typedef struct itemNode
{
    char name[MAXLEN];
    int count;
    struct itemNode *left, *right;
} itemNode;

typedef struct treeNameNode
{
    char treeName[MAXLEN];
    struct treeNameNode *left, *right;
    itemNode *theTree;
} treeNameNode;

FILE *outputFile;

treeNameNode* createTreeNameNode(char *name)
{
    treeNameNode *tree = (treeNameNode*) malloc(sizeof(treeNameNode));
    tree->left = tree->right = NULL;
    tree->theTree = NULL;
    strcpy(tree->treeName, name);
    return tree;
}

treeNameNode* insertNameNode(treeNameNode *root, treeNameNode *node)
{
    if (root == NULL)
    {
        return node;
    }

    else
    {
        if (0 >= strcmp(node->treeName, root->treeName))
        {
            if (root->left != NULL)
            {
                root->left = insertNameNode(root->left, node);
            }
            else
            {
                root->left = node;
            }
        }

        else
        {
            if (root->right != NULL)
            {
                root->right = insertNameNode(root->right, node);
            }
            else
            {
                root->right = node;
            }
        }
        return root;
    }
}

treeNameNode* buildNameTree(treeNameNode *root, FILE *input, int nodeCnt)
{
    treeNameNode *tmp;
    for (int i = 0; i < nodeCnt; i++)
    {
        char name[MAXLEN];
        fscanf(input, "%s", name);
        tmp = createTreeNameNode(name);
        root = insertNameNode(root, tmp);
    }
    return root;
}

itemNode* createItemTreeNode(char *treeName, char *itemName, int count)
{
    itemNode *item = (itemNode*) malloc(sizeof(itemNode));
    item->count = count;
    item->left = item->right = NULL;
    strcpy(item->name, itemName);
    return item;
}

void printTreeInOderByName(treeNameNode *root)
{
    if (root != NULL)
    {
        printTreeInOderByName(root->left);
        fprintf(outputFile, "%s ", root->treeName);
        printTreeInOderByName(root->right);
    }
}

void printTreeInOderByItem(itemNode *root)
{
    if (root != NULL)
    {
        printTreeInOderByItem(root->left);
        fprintf(outputFile, "%s ", root->name);
        printTreeInOderByItem(root->right);
    }
}

void printOderByItemFormat(treeNameNode *root)
{
    if (root != NULL)
    {
        printOderByItemFormat(root->left);
        fprintf(outputFile, "===%s===\n", root->treeName);
        printTreeInOderByItem(root->theTree);
        fprintf(outputFile, "\n");
        printOderByItemFormat(root->right);
    }
}

void traverse_in_traverse(treeNameNode *root)
{
    printTreeInOderByName(root);
    fprintf(outputFile, "\n");
    printOderByItemFormat(root);
}

treeNameNode* searchNameNode(treeNameNode *root, char name[MAXLEN])
{
    if (root == NULL)
    {
        return 0;
    }
    
    else
    {
        if (strcmp(root->treeName, name) == 0)
        {
            return root;
        }
        else if (strcmp(root->treeName, name) > 0)
        {
            return searchNameNode(root->left, name);
        }
        else
        {
            return searchNameNode(root->right, name);
        }
    }
}

itemNode* insertItem(itemNode *root, itemNode *item)
{
    if (root == NULL)
    {
        return item;
    }
    else
    {
        if (strcmp(item->name, root->name) > 0)
        {
            if (root->right != NULL)
            {
            root->right = insertItem(root->right, item);
            }
            else
            {
            root->right = item;
            }
        }

        else
        {
            if (root->left != NULL)
            {
            root->left = insertItem(root->left, item);
            }
            else
            {
            root->left = item;
            }
        }
        return root;
    }
}

treeNameNode* buildTreeByItemFromInputFile(treeNameNode *root, FILE *input, int size)
{
    itemNode *item;
    for (int j = 0; j < size; j++)
    {
        char name[MAXLEN];
        char nameItem[MAXLEN];
        int count;
        fscanf(input, "%s %s %d\n", name, nameItem, &count);
        item = createItemTreeNode(name, nameItem, count);
        treeNameNode *temp = searchNameNode(root, name);
        temp->theTree = insertItem(temp->theTree, item);
    }
    return root;
}

void deleteItem(itemNode *root)
{
    if (root != NULL)
    {
        deleteItem(root->left);
        deleteItem(root->right);
        free(root);
    }
}

void deleteTree(treeNameNode *root)
{
    if (root != NULL)
    {
        if (root->theTree != NULL)
        deleteItem(root->theTree);
        deleteTree(root->left);
        deleteTree(root->right);
        free(root);
    }
}

itemNode* searchItemNode(itemNode *root, char name[MAXLEN])
{
    if (root == NULL)
    {
        return 0;
    }
    else
    {
        if (strcmp(root->name, name) == 0)
        {
            return root;
        }
        else if (strcmp(root->name, name) > 0)
        {
            return searchItemNode(root->left, name);
        }
        else
        {
            return searchItemNode(root->right, name);
        }
    }
}

void findTheCommandFromInputFile(treeNameNode *root, FILE *input)
{
    char name[MAXLEN];
    char nameItem[MAXLEN];
    fscanf(input, "%s %s", name, nameItem);
    treeNameNode *node = searchNameNode(root, name);
    if (node == 0)
    {
        fprintf(outputFile, "%s does not exist\n", name);
    }
    else
    {
        itemNode *tempItemNode;
        tempItemNode = searchItemNode(node->theTree, nameItem);
        if (tempItemNode == 0)
        fprintf(outputFile, "%s not found in %s\n", nameItem, node->treeName);
        else
        fprintf(outputFile, "%d %s found in %s\n", tempItemNode->count, tempItemNode->name, node->treeName);
    }
}

int countItemBefore(itemNode *root, char names[MAXLEN])
{
    int cnt = 0;
    if (root == NULL)
    {
        return 0;
    }
    else if (strcmp(root->name, names) < 0)
    {
        cnt++;
        cnt += countItemBefore(root->left, names);
        cnt += countItemBefore(root->right, names);
    }
    else
    {
        cnt += countItemBefore(root->left, names);
    }
    return cnt;
}

void handleBeforeCommand(treeNameNode *root, FILE *input)
{
    char name[MAXLEN];
    char nameItem[MAXLEN];
    fscanf(input, "%s %s", name, nameItem);
    treeNameNode *tmp = searchNameNode(root, name);
    itemNode *node = searchItemNode(tmp->theTree, nameItem);
    int count = 0;
    count = countItemBefore(tmp->theTree, nameItem);
    fprintf(outputFile, "item before %s: %d\n", node->name, count);
}

int getHeightTree(itemNode *root)
{
    int leftH = 0;
    int rightH = 0;
    if (root == NULL)
    {
        return -1;
    }
    leftH = getHeightTree(root->left);
    rightH = getHeightTree(root->right);
    if(leftH>rightH)
    {
        return 1+leftH;
    }
    else
    {
        return 1+rightH;
    }

}

void handleBalanceCommand(treeNameNode *root, FILE *input)
{
    char names[MAXLEN];
    fscanf(input, "%s", names);
    treeNameNode *nodes = searchNameNode(root, names);
    int left_height = getHeightTree(nodes->theTree->left);
    int right_height = getHeightTree(nodes->theTree->right);
    int diff = abs(left_height - right_height);
    if (diff >= 2)
    {
        fprintf(outputFile, "%s: left height %d, right height %d, difference %d, not balanced\n", nodes->treeName, left_height,
        right_height, diff);
    }
    else
    {
        fprintf(outputFile, "%s: left height %d, right height %d, difference %d, balanced\n", nodes->treeName, left_height, right_height, diff);
    }
}

int getTotalCount(itemNode *root)
{
    int sum = 0;
    if (root != NULL)
    {
        sum += getTotalCount(root->left);
        sum += getTotalCount(root->right);
        return sum + root->count;
    }
    return 0;
}

void handleCountCommand(treeNameNode *root, FILE *input)
{
    char names[MAXLEN];
    fscanf(input, "%s", names);
    treeNameNode *node = searchNameNode(root, names);
    int sum = getTotalCount(node->theTree);
    fprintf(outputFile, "%s count %d\n", node->treeName, sum);
}

treeNameNode* getMinNodeByName(treeNameNode *root)
{
    treeNameNode *node = root;
    while (node && node->left != NULL)
    {
        node = node->left;
    }
    return node;
}

itemNode* getMinNodeByItem(itemNode *root)
{
    itemNode *node = root;
    while (node && node->left != NULL)
    {
        node = node->left;
    }
    return node;
}

itemNode* deleteItemNode(itemNode *root, char names[MAXLEN])
{
    if (root == NULL)
    {
        return root;
    }
    if (strcmp(names, root->name) < 0)
    {
        root->left = deleteItemNode(root->left, names);
    }
    else if (strcmp(names, root->name) > 0)
    {
        root->right = deleteItemNode(root->right, names);
    }
    else
    {
        if (root->left == NULL)
        {
            itemNode *tmp = root->right;
            free(root);
            return tmp;
        }
        else if (root->right == NULL)
        {
            itemNode *tmp = root->left;
            free(root);
            return tmp;
        }
        itemNode *tmp = getMinNodeByItem(root->right);
        strcpy(root->name, tmp->name);
        root->count = tmp->count;
        root->right = deleteItemNode(root->right, tmp->name);
    }
    return root;
}

void handleDeleteCommand(treeNameNode *root, FILE *input)
{
    char name[MAXLEN];
    char nameItem[MAXLEN];
    fscanf(input, "%s %s", name, nameItem);
    treeNameNode *node = searchNameNode(root, name);
    itemNode *outputItem = searchItemNode(node->theTree, nameItem);
    char deleteNames[MAXLEN];
    strcpy(deleteNames, outputItem->name);
    node->theTree = deleteItemNode(node->theTree, nameItem);
    fprintf(outputFile, "%s deleted from %s\n", deleteNames, node->treeName);
}

treeNameNode* deleteNameNode(treeNameNode *root, char names[MAXLEN])
{
    if (root == NULL)
    {
        return root;
    }
    if (strcmp(names, root->treeName) < 0)
    {
        root->left = deleteNameNode(root->left, names);
    }
    else if (strcmp(names, root->treeName) > 0)
    {
        root->right = deleteNameNode(root->right, names);
    }
    else
    {
        if (root->left == NULL)
        {
            treeNameNode *tmp = root->right;
            free(root);
            return tmp;
        }
        else if (root->right == NULL)
        {
            treeNameNode *tmp = root->left;
            free(root);
            return tmp;
        }
        treeNameNode *tmp = getMinNodeByName(root->right);
        strcpy(root->treeName, tmp->treeName);
        root->right = deleteNameNode(root->right, tmp->treeName);
    }
    return root;
}

void handleDeleteNameCommand(treeNameNode *root, FILE *input)
{
    char names[MAXLEN];
    fscanf(input, "%s", names);
    treeNameNode *node = searchNameNode(root, names);
    char treeNameFromTree[MAXLEN];
    strcpy(treeNameFromTree, node->treeName);
    deleteItem(node->theTree);
    root = deleteNameNode(root, names);
    fprintf(outputFile, "%s deleted\n", treeNameFromTree);
}

void handleReduceCommand(treeNameNode *root, FILE *input)
{
    char names[MAXLEN];
    char nameItem[MAXLEN];
    int reduce;
    fscanf(input, "%s %s %d", names, nameItem, &reduce);
    treeNameNode *node = searchNameNode(root, names);
    itemNode *items = searchItemNode(node->theTree, nameItem);
    items->count -= reduce;
    if (items->count <= 0)
    {
        fprintf(outputFile, "%s reduced\n", items->name);
        node->theTree = deleteItemNode(node->theTree, nameItem);
    }
    fprintf(outputFile, "%s reduced\n", items->name);
}

treeNameNode* handleCommand(treeNameNode *root, FILE *input, int Q)
{
    for (int k = 0; k < Q; k++)
    {
        char querie[MAXLEN];
        fscanf(input, "%s", querie);
        if (strcmp(querie, "search") == 0)
        findTheCommandFromInputFile(root, input);
        
        else if (strcmp(querie, "item_before") == 0)
        handleBeforeCommand(root, input);
        
        else if (strcmp(querie, "height_balance") == 0)
        handleBalanceCommand(root, input);
        
        else if (strcmp(querie, "count") == 0)
        handleCountCommand(root, input);
        
        else if (strcmp(querie, "delete") == 0)
        handleDeleteCommand(root, input);
        
        else if (strcmp(querie, "delete_name") == 0)
        handleDeleteNameCommand(root, input);
        
        else if (strcmp(querie, "reduce") == 0)
        handleReduceCommand(root, input);
    }
    return root;
}

int main(int argc, char **argv)
{
    FILE *input = fopen("in.txt", "r");
    outputFile = fopen("out.txt", "w");
    if (input == NULL || outputFile == NULL)
    {
        printf("ERROR when open the input and output file.\nPlease check the input: in.txt.\n");
        return 1;
    }
    treeNameNode *root = NULL;
    int N, I, Q;
    fscanf(input, "%d %d %d", &N, &I, &Q);
    root = buildNameTree(root, input, N);
    root = buildTreeByItemFromInputFile(root, input, I);
    traverse_in_traverse(root);
    root = handleCommand(root, input, Q);
    deleteTree(root);
    fclose(input);
    fclose(outputFile);
    return 0;
}