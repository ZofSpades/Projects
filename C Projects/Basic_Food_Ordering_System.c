#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define MAX_MENU_ITEMS 10
#define MAX_CART_ITEMS 10

struct User {
    char name[20];
};

struct Restaurant {
    char name[50];
    char address[100];
    char menuItems[MAX_MENU_ITEMS][50];
    float prices[MAX_MENU_ITEMS];
    int menuSize;
};

struct CartItem {
    int itemNumber;
};

struct Cart {
    struct CartItem items[MAX_CART_ITEMS];
    int itemCount;
};

int containsDigit(const char *str) {
    while (*str) {
        if (*str>='0' && *str<='9') {
            return 1;
        }
        str++;
    }
    return 0;
}

void displayMenu() {
    printf("\n1. New User\n");
    printf("2. Restaurant Details\n");  
    printf("3. Add to Cart\n");
    printf("4. Show Cart\n");
    printf("5. Payment(Simulation)\n");
    printf("6. Delivery(Simulation)\n");
    printf("0. Exit\n");
    printf("Enter your choice: ");
}

void handleNewUser(struct User *user) {
    char tempUsername[20];
    printf("Enter a username: ");   
    scanf(" %[^\n]s", tempUsername);
    if (!containsDigit(tempUsername)) {
        strcpy(user->name, tempUsername);
        printf("New user %s created successfully!\n", user->name);
    } else {
        printf("Username cannot contain numbers. Please try again.\n");
    }
}

void handleRestaurantDetails(struct Restaurant *restaurant) {
    printf("Enter restaurant name: ");
    scanf(" %[^\n]s", restaurant->name);
    printf("Enter restaurant address: ");
    scanf(" %[^\n]s", restaurant->address);
    printf("Restaurant Name: %s\n", restaurant->name);
    printf("Address: %s\n", restaurant->address);
    printf("\nEnter the number of items in the menu: ");
    scanf("%d", &restaurant->menuSize);
    printf("\nEnter menu details:\n");
    for (int i = 0; i < restaurant->menuSize; i++) {
        printf("Item %d: ", i + 1);
        scanf(" %[^\n]s", restaurant->menuItems[i]);
        printf("Price: ");
        scanf("%f", &restaurant->prices[i]);
    }
    printf("\nMenu:\n");
    for (int i = 0; i < restaurant->menuSize; i++) {
        printf("%d. %s - %.2f\n", i + 1, restaurant->menuItems[i], restaurant->prices[i]);
    }
}

void addToCart(struct Restaurant *restaurant, struct Cart *cart) {
    int item;
    printf("Enter item number to add to cart: ");
    scanf("%d", &item);
    if (item == 0) {
        return;
    } else if (item >= 1 && item <= restaurant->menuSize) {
        if (cart->itemCount < MAX_CART_ITEMS) {
            cart->items[cart->itemCount].itemNumber = item;
            cart->itemCount++;
            printf("Item added to cart!\n");
        } else {
            printf("Cart is full!\n");
        }
    } else {
        printf("Invalid item number.\n");
    }
}

void showCart(struct Restaurant *restaurant, struct Cart *cart) {
    printf("\nCart:\n");
    for (int i = 0; i < cart->itemCount; i++) {
        printf("%d. %s - %.2f\n", i + 1, restaurant->menuItems[cart->items[i].itemNumber - 1],restaurant->prices[cart->items[i].itemNumber - 1]);
    }
}

void handlePayment(struct Cart *cart, struct Restaurant *restaurant) {
    float total = 0;
    printf("\nOrders:\n");
    for (int i = 0; i < cart->itemCount; i++) {
        printf("%d. %s - %.2f\n", i + 1, restaurant->menuItems[cart->items[i].itemNumber - 1], restaurant->prices[cart->items[i].itemNumber - 1]);
        total += restaurant->prices[cart->items[i].itemNumber - 1];
    }
    sleep(2);
    printf("Total Amount: %.2f\n", total);
    cart->itemCount = 0;
    sleep(3);
    printf("Payment successful!\n");
}

void handleDelivery() {
    printf("Your order will be delivered soon!\n");
    sleep(2);
    printf("Your order is out for delivery!\n");
    sleep(3);
    printf("Your order is delivered.\nThanks for ordering!\n");
}

int main() {
    int choice;
    struct User user;
    struct Restaurant restaurant;
    struct Cart cart = { .itemCount = 0 };
    printf("Welcome!");
    do {
        displayMenu();
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                handleNewUser(&user);
                break;
            case 2:
                handleRestaurantDetails(&restaurant);
                break;
            case 3:
                addToCart(&restaurant, &cart);
                break;
            case 4:
                showCart(&restaurant, &cart);
                break;
            case 5:
                handlePayment(&cart, &restaurant);
                break;
            case 6:
                handleDelivery();
                break;
            case 0:
                printf("Exiting program...\n");
                break;
            default:
                printf("Invalid choice\n");
        }
    } while (choice != 0);
    return 0;
}