#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE 100

// Define the structure for a node in the hash table and adjacency list
typedef struct Node {
    int data;
    int flag; // 0: empty, 1: occupied
    struct Node* next;
} NODE;

// Define the structure for a user
typedef struct User {
    char username[100];
    NODE browsingHistory[SIZE];             
    NODE purchaseHistory[SIZE];
} USER;

// Define the structure for a product
typedef struct Product {
    int productID;
    char productName[100];
} PRODUCT;

// Define the structure for a graph
typedef struct Graph {
    int numVertices;
    NODE** adjLists;
} GRAPH;

// Declare global variables for user and product tables
NODE userTable[SIZE];
PRODUCT productTable[SIZE];

// Initialize the hash table
void initializeHashTable(NODE *hashTable) {
    for (int i = 0; i < SIZE; i++) {
        hashTable[i].data = 0;
        hashTable[i].flag = 0;
        hashTable[i].next = NULL;
    }
}

// Initialize the product table
void initializeProductTable(PRODUCT *productTable) {
    for (int i = 0; i < SIZE; i++) {
        productTable[i].productID = 0;
        strcpy(productTable[i].productName, "");
    }
}

// Generate a key for a given name
int generateKey(char *name) {
    int key = 0;
    for (int i = 0; name[i] != '\0'; i++) {
        key += name[i];
    }
    return key;
}

// Insert a key into the hash table
void insert(int key, NODE *hashTable) {
    int i = 0;
    int index = ((key % SIZE) + i) % SIZE;
    while (hashTable[index].flag != 0 && i < SIZE) {
        i++;
        index = ((key % SIZE) + i) % SIZE;
    }
    if (hashTable[index].flag == 0) {
        hashTable[index].data = key;
        hashTable[index].flag = 1;
    }
}

// Insert a user into the user table
void insertUser(char *name, NODE *hashTable) {
    int key = generateKey(name);
    insert(key, hashTable);
}

// Insert a product into the product table
void insertProduct(int productID, char *productName, PRODUCT *productTable) {
    for (int i = 0; i < SIZE; i++) {
        if (productTable[i].productID == 0) {
            productTable[i].productID = productID;
            strcpy(productTable[i].productName, productName);
            break;
        }
    }
}

// Check if a product exists in the product table
int productExists(int productID, PRODUCT *productTable) {
    for (int i = 0; i < SIZE; i++) {
        if (productTable[i].productID == productID) {
            return 1;
        }
    }
    return 0;
}

// Check if a user exists in the user table
int userExists(char *username, NODE *userTable) {
    int key = generateKey(username);
    int i = 0;
    int index = ((key % SIZE) + i) % SIZE;
    while (i < SIZE) {
        if (userTable[index].data == key && userTable[index].flag == 1) {
            return 1;
        }
        i++;
        index = ((key % SIZE) + i) % SIZE;
    }
    return 0;
}

// Create a graph with a given number of vertices
GRAPH* createGraph(int vertices) {
    GRAPH* graph = malloc(sizeof(GRAPH));
    graph->numVertices = vertices;
    graph->adjLists = malloc(vertices * sizeof(NODE*));

    for (int i = 0; i < vertices; i++) {
        graph->adjLists[i] = NULL;
    }

    return graph;
}

// Create a new node
NODE* createNode(int data) {
    NODE* newNode = malloc(sizeof(NODE));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// Add an edge to the graph
void addEdge(GRAPH* graph, int src, int dest) {
    NODE* newNode = createNode(dest);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;
}

// Track the browsing history of a user
void trackBrowsingHistory(GRAPH* graph, char* username, int productID) {
    printf("Tracking browsing history for user: %s\n", username);
    int userKey = generateKey(username) % SIZE;
    addEdge(graph, userKey, productID);
}

// Track the purchase history of a user
void trackPurchaseHistory(USER *user, int productID) {
    int key = productID % SIZE;
    user->purchaseHistory[key].data = productID;
    user->purchaseHistory[key].flag = 1;
}

// Generate recommendations for a user based on browsing and purchase history
void generateRecommendations(USER *user, GRAPH* graph) {
    printf("Generating recommendations for %s...\n", user->username);
    int userKey = generateKey(user->username) % SIZE;
    NODE* temp = graph->adjLists[userKey];
    int recommendedProducts[SIZE] = {0};

    // Traverse the user's browsing history
    while (temp) {
        int productID = temp->data;
        NODE* productNode = graph->adjLists[productID];
        while (productNode) {
            recommendedProducts[productNode->data]++;
            productNode = productNode->next;
        }
        temp = temp->next;
    }

    // Traverse the user's purchase history
    for (int i = 0; i < SIZE; i++) {
        if (user->purchaseHistory[i].flag == 1) {
            int productID = user->purchaseHistory[i].data;
            NODE* productNode = graph->adjLists[productID];
            while (productNode) {
                recommendedProducts[productNode->data]++;
                productNode = productNode->next;
            }
        }
    }

    // Display recommended products
    printf("Recommended products:\n");
    for (int i = 0; i < SIZE; i++) {
        if (recommendedProducts[i] > 0) {
            for (int j = 0; j < SIZE; j++) {
                if (productTable[j].productID == i) {
                    printf("Product ID: %d, Product Name: %s, Score: %d\n", i, productTable[j].productName, recommendedProducts[i]);
                    break;
                }
            }
        }
    }
}

// Simulate a user browsing a product
void browseProduct(GRAPH* graph, char* username, int productID, PRODUCT *productTable) {
    if (!productExists(productID, productTable)) {
        printf("Product ID %d does not exist.\n", productID);
        return;
    }
    printf("User %s is browsing product %d\n", username, productID);
    trackBrowsingHistory(graph, username, productID);
}

// Generate random suggestions for a user based on browsing and purchase history
void generateRandomSuggestions(GRAPH* graph, USER* user) {
    printf("Generating random suggestions for user: %s...\n", user->username);
    int userKey = generateKey(user->username) % SIZE;
    NODE* temp = graph->adjLists[userKey];
    int visitedProducts[SIZE] = {0};
    int count = 0;

    // Collect all products the user has visited
    while (temp) {
        visitedProducts[count++] = temp->data;
        temp = temp->next;
    }

    // Collect all products the user has purchased
    for (int i = 0; i < SIZE; i++) {
        if (user->purchaseHistory[i].flag == 1) {
            visitedProducts[count++] = user->purchaseHistory[i].data;
        }
    }

    // Generate random suggestions from visited and purchased products
    if (count > 0) {
        srand(time(0));
        for (int i = 0; i < 2; i++) {
            int randomIndex = rand() % count;
            int productID = visitedProducts[randomIndex];
            for (int j = 0; j < SIZE; j++) {
                if (productTable[j].productID == productID) {
                    printf("Suggestion %d: Product ID %d, Product Name: %s\n", i + 1, productID, productTable[j].productName);
                    break;
                }
            }
        }
    } else {
        printf("No products visited or purchased yet.\n");
    }
}

// Show the browsing history of a user
void showBrowsingHistory(GRAPH* graph, char* username) {
    printf("Browsing history for user: %s\n", username);
    int userKey = generateKey(username) % SIZE;
    NODE* temp = graph->adjLists[userKey];

    if (temp == NULL) {
        printf("No browsing history found for user: %s\n", username);
        return;
    }

    while (temp) {
        printf("Visited Product ID: %d\n", temp->data);
        temp = temp->next;
    }
}

// Show the purchase history of a user
void showPurchaseHistory(USER *user) {
    printf("Purchase history for user: %s\n", user->username);
    for (int i = 0; i < SIZE; i++) {
        if (user->purchaseHistory[i].flag == 1) {
            printf("Purchased Product ID: %d\n", user->purchaseHistory[i].data);
        }
    }
}

// Simulate a user purchasing a product
void purchaseProduct(USER *user, int productID, PRODUCT *productTable) {
    if (!productExists(productID, productTable)) {
        printf("Product ID %d does not exist.\n", productID);
        return;
    }
    printf("User %s is purchasing product %d\n", user->username, productID);
    trackPurchaseHistory(user, productID);
}

// Retrieve user data including browsing and purchase history
void retrieveUserData(GRAPH* graph, USER* user) {
    printf("User data for: %s\n", user->username);
    showBrowsingHistory(graph, user->username);
    showPurchaseHistory(user);
}

// Retrieve all products in the product table
void retrieveAllProducts(PRODUCT *productTable) {
    printf("All products:\n");
    for (int i = 0; i < SIZE; i++) {
        if (productTable[i].productID != 0) {
            printf("Product ID: %d, Product Name: %s\n", productTable[i].productID, productTable[i].productName);
        }
    }
}

int main() {
    initializeHashTable(userTable);
    initializeProductTable(productTable);

    GRAPH* graph = createGraph(SIZE);

    // Hardcode some users
    insertUser("varun", userTable);
    insertUser("vatsal", userTable);
    insertUser("vedanta", userTable);
    insertUser("vrishabh", userTable);

    // Hardcode some products
    insertProduct(101, "Smartphone", productTable);
    insertProduct(102, "Laptop", productTable);
    insertProduct(103, "Plushies", productTable);

    while (1) {
        int n, productID;
        char name[30];
        char productName[100];
        USER user;

        printf("\n\nEnter your option\n");
        printf("1. Add user\n");
        printf("2. Add product\n");
        printf("3. Browse product\n");
        printf("4. Show browsing history\n");
        printf("5. Purchase product\n");
        printf("6. Show purchase history\n");
        printf("7. Generate random suggestions\n");
        printf("8. Retrieve user data\n");
        printf("9. Retrieve product data\n");
        printf("10. Exit\n");
        scanf("%d", &n);

        switch (n) {
            case 1:
                printf("Enter username: ");
                scanf("%s", name);
                insertUser(name, userTable);
                break;
            case 2:
                printf("Enter product ID: ");
                scanf("%d", &productID);
                printf("Enter product name: ");
                getchar(); // Consume the newline character left by scanf
                fgets(productName, sizeof(productName), stdin);
                productName[strcspn(productName, "\n")] = '\0'; // Remove the newline character
                insertProduct(productID, productName, productTable);
                break;
            case 3:
                printf("Enter username: ");
                scanf("%s", name);
                if (!userExists(name, userTable)) {
                    printf("No such user exists.\n");
                    break;
                }
                printf("Enter product ID: ");
                scanf("%d", &productID);
                browseProduct(graph, name, productID, productTable);
                break;
            case 4:
                printf("Enter username: ");
                scanf("%s", name);
                if (!userExists(name, userTable)) {
                    printf("No such user exists.\n");
                    break;
                }
                showBrowsingHistory(graph, name);
                break;
            case 5:
                printf("Enter username: ");
                scanf("%s", name);
                if (!userExists(name, userTable)) {
                    printf("No such user exists.\n");
                    break;
                }
                printf("Enter product ID: ");
                scanf("%d", &productID);
                strcpy(user.username, name);
                purchaseProduct(&user, productID, productTable);
                break;
            case 6:
                printf("Enter username: ");
                scanf("%s", name);
                if (!userExists(name, userTable)) {
                    printf("No such user exists.\n");
                    break;
                }
                strcpy(user.username, name);
                showPurchaseHistory(&user);
                break;
            case 7:
                printf("Enter username: ");
                scanf("%s", name);
                if (!userExists(name, userTable)) {
                    printf("No such user exists.\n");
                    break;
                }
                strcpy(user.username, name);
                generateRandomSuggestions(graph, &user);
                break;
            case 8:
                printf("Enter username: ");
                scanf("%s", name);
                if (!userExists(name, userTable)) {
                    printf("No such user exists.\n");
                    break;
                }
                strcpy(user.username, name);
                retrieveUserData(graph, &user);
                break;
            case 9:
                retrieveAllProducts(productTable);
                break;
            case 10:
                exit(0);
        }
    }

    return 0;
}