#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Union for different product data formats
typedef union {
    int int_code;        // integer code
    char str_code[10];   // string code
} ProductCode;

// Structure for storing product information
typedef struct {
    char name[50];       // product name
    int quantity;        // quantity
    int workshop;        // workshop number
    ProductCode code;    // product code (union)
} Product;

// Global variables
Product* products = NULL;  // dynamic array of structures
int product_count = 0;     // number of records
int capacity = 0;          // array capacity

// Function prototypes
void init_data();
void add_product();
void delete_product();
void edit_product();
void view_all();
void view_workshop_stats();
void save_to_file();
void load_from_file();
void edit_in_file();
void free_memory();
void clear_input_buffer();  // New function to clear input buffer

int main() {
    int choice;

    // Load data from file on startup
    load_from_file();

    do {
        printf("\n========== MENU ==========\n");
        printf("1. Initialize data\n");
        printf("2. Add record\n");
        printf("3. Delete record\n");
        printf("4. Edit record\n");
        printf("5. View all records\n");
        printf("6. Workshop statistics\n");
        printf("7. Save to file\n");
        printf("8. Edit in file\n");
        printf("0. Exit\n");
        printf("==========================\n");
        printf("Select action: ");
        scanf("%d", &choice);
        clear_input_buffer();  // Clear buffer after scanf

        switch (choice) {
        case 1:
            init_data();
            break;
        case 2:
            add_product();
            break;
        case 3:
            delete_product();
            break;
        case 4:
            edit_product();
            break;
        case 5:
            view_all();
            break;
        case 6:
            view_workshop_stats();
            break;
        case 7:
            save_to_file();
            break;
        case 8:
            edit_in_file();
            break;
        case 0:
            save_to_file(); // Auto-save on exit
            free_memory();
            break;
        default:
            printf("Invalid choice!\n");
        }
    } while (choice != 0);

    return 0;
}

// Clear input buffer
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Initialize data
void init_data() {
    free_memory(); // Free old data

    printf("Enter number of records to initialize: ");
    scanf("%d", &capacity);
    clear_input_buffer();

    products = (Product*)malloc(capacity * sizeof(Product));
    if (!products) {
        printf("Memory allocation error!\n");
        exit(1);
    }

    printf("Initializing %d records:\n", capacity);
    for (int i = 0; i < capacity; i++) {
        printf("\nRecord %d:\n", i + 1);

        // Input name (can contain spaces)
        printf("Name: ");
        fgets(products[i].name, sizeof(products[i].name), stdin);
        products[i].name[strcspn(products[i].name, "\n")] = 0; // Remove newline

        // Input quantity
        printf("Quantity: ");
        scanf("%d", &products[i].quantity);
        clear_input_buffer();

        // Input workshop number
        printf("Workshop number: ");
        scanf("%d", &products[i].workshop);
        clear_input_buffer();

        // Input code type
        int code_type;
        printf("Code type (1 - number, 2 - string): ");
        scanf("%d", &code_type);
        clear_input_buffer();

        if (code_type == 1) {
            printf("Integer code: ");
            scanf("%d", &products[i].code.int_code);
            clear_input_buffer();
        }
        else {
            printf("String code: ");
            fgets(products[i].code.str_code, sizeof(products[i].code.str_code), stdin);
            products[i].code.str_code[strcspn(products[i].code.str_code, "\n")] = 0;
        }
    }
    product_count = capacity;
    printf("Initialization complete!\n");
}

// Add new record
void add_product() {
    if (product_count >= capacity) {
        capacity = capacity == 0 ? 5 : capacity * 2;
        products = (Product*)realloc(products, capacity * sizeof(Product));
        if (!products) {
            printf("Memory allocation error!\n");
            exit(1);
        }
    }

    printf("\nAdding new record:\n");

    // Input name
    printf("Name: ");
    fgets(products[product_count].name, sizeof(products[product_count].name), stdin);
    products[product_count].name[strcspn(products[product_count].name, "\n")] = 0;

    // Input quantity
    printf("Quantity: ");
    scanf("%d", &products[product_count].quantity);
    clear_input_buffer();

    // Input workshop number
    printf("Workshop number: ");
    scanf("%d", &products[product_count].workshop);
    clear_input_buffer();

    // Input code type
    int code_type;
    printf("Code type (1 - number, 2 - string): ");
    scanf("%d", &code_type);
    clear_input_buffer();

    if (code_type == 1) {
        printf("Integer code: ");
        scanf("%d", &products[product_count].code.int_code);
        clear_input_buffer();
    }
    else {
        printf("String code: ");
        fgets(products[product_count].code.str_code,
            sizeof(products[product_count].code.str_code), stdin);
        products[product_count].code.str_code[strcspn(
            products[product_count].code.str_code, "\n")] = 0;
    }

    product_count++;
    printf("Record added!\n");
}

// Delete record
void delete_product() {
    if (product_count == 0) {
        printf("No records to delete!\n");
        return;
    }

    int index;
    printf("Enter record number to delete (1-%d): ", product_count);
    scanf("%d", &index);
    clear_input_buffer();

    if (index < 1 || index > product_count) {
        printf("Invalid record number!\n");
        return;
    }

    index--; // Switch to 0-indexing

    // Shift records one position
    for (int i = index; i < product_count - 1; i++) {
        products[i] = products[i + 1];
    }

    product_count--;
    printf("Record deleted!\n");
}

// Edit record
void edit_product() {
    if (product_count == 0) {
        printf("No records to edit!\n");
        return;
    }

    int index;
    printf("Enter record number to edit (1-%d): ", product_count);
    scanf("%d", &index);
    clear_input_buffer();

    if (index < 1 || index > product_count) {
        printf("Invalid record number!\n");
        return;
    }

    index--; // Switch to 0-indexing

    printf("\nEditing record %d:\n", index + 1);

    // Edit name
    printf("Current name: %s\n", products[index].name);
    printf("New name (press Enter to keep current): ");
    char input[100];
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0) {
        strcpy(products[index].name, input);
    }

    // Edit quantity
    printf("Current quantity: %d\n", products[index].quantity);
    printf("New quantity (press Enter to keep current): ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0) {
        products[index].quantity = atoi(input);
    }

    // Edit workshop number
    printf("Current workshop number: %d\n", products[index].workshop);
    printf("New workshop number (press Enter to keep current): ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0) {
        products[index].workshop = atoi(input);
    }

    // Edit product code
    printf("Current product code: ");
    if (products[index].code.int_code != 0) {
        printf("%d\n", products[index].code.int_code);
    }
    else {
        printf("%s\n", products[index].code.str_code);
    }

    printf("New product code (press Enter to keep current): ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0) {
        // Check if input is a number
        int is_number = 1;
        for (int i = 0; input[i] != '\0'; i++) {
            if (input[i] < '0' || input[i] > '9') {
                is_number = 0;
                break;
            }
        }

        if (is_number) {
            products[index].code.int_code = atoi(input);
            // Clear string code if switching to integer
            products[index].code.str_code[0] = '\0';
        }
        else {
            strcpy(products[index].code.str_code, input);
            // Clear integer code if switching to string
            products[index].code.int_code = 0;
        }
    }

    printf("Record updated!\n");
}

// View all records
void view_all() {
    if (product_count == 0) {
        printf("No records to display!\n");
        return;
    }

    printf("\n========== ALL RECORDS ==========\n");
    printf("%-3s %-20s %-10s %-8s %-10s\n",
        "ID", "Name", "Quantity", "Workshop", "Code");
    printf("===================================================\n");

    for (int i = 0; i < product_count; i++) {
        char code_str[20];
        // Determine code type for display
        if (products[i].code.int_code != 0) {
            sprintf(code_str, "%d", products[i].code.int_code);
        }
        else {
            strcpy(code_str, products[i].code.str_code);
        }

        printf("%-3d %-20s %-10d %-8d %-10s\n",
            i + 1, products[i].name, products[i].quantity,
            products[i].workshop, code_str);
    }
    printf("===================================================\n");
}

// Workshop statistics (quantity by name in descending order)
void view_workshop_stats() {
    if (product_count == 0) {
        printf("No records for analysis!\n");
        return;
    }

    int workshop_num;
    printf("Enter workshop number for statistics: ");
    scanf("%d", &workshop_num);
    clear_input_buffer();

    // Create temporary array for statistics
    typedef struct {
        char name[50];
        int total_quantity;
    } WorkshopStat;

    WorkshopStat* stats = NULL;
    int stat_count = 0;

    // Collect workshop statistics
    for (int i = 0; i < product_count; i++) {
        if (products[i].workshop == workshop_num) {
            int found = 0;

            // Search for name in statistics
            for (int j = 0; j < stat_count; j++) {
                if (strcmp(stats[j].name, products[i].name) == 0) {
                    stats[j].total_quantity += products[i].quantity;
                    found = 1;
                    break;
                }
            }

            // If not found, add new record
            if (!found) {
                stat_count++;
                stats = (WorkshopStat*)realloc(stats, stat_count * sizeof(WorkshopStat));
                strcpy(stats[stat_count - 1].name, products[i].name);
                stats[stat_count - 1].total_quantity = products[i].quantity;
            }
        }
    }

    if (stat_count == 0) {
        printf("No data for workshop %d!\n", workshop_num);
        free(stats);
        return;
    }

    // Sort by descending quantity
    for (int i = 0; i < stat_count - 1; i++) {
        for (int j = i + 1; j < stat_count; j++) {
            if (stats[i].total_quantity < stats[j].total_quantity) {
                WorkshopStat temp = stats[i];
                stats[i] = stats[j];
                stats[j] = temp;
            }
        }
    }

    // Display results
    printf("\n========== STATISTICS FOR WORKSHOP %d ==========\n", workshop_num);
    printf("%-20s %-15s\n", "Product Name", "Total Quantity");
    printf("==========================================\n");

    for (int i = 0; i < stat_count; i++) {
        printf("%-20s %-15d\n", stats[i].name, stats[i].total_quantity);
    }
    printf("==========================================\n");

    free(stats);
}

// Save to file
void save_to_file() {
    FILE* file = fopen("products.txt", "w");
    if (!file) {
        printf("Error opening file for writing!\n");
        return;
    }

    fprintf(file, "%d\n", product_count);
    for (int i = 0; i < product_count; i++) {
        fprintf(file, "%s\n", products[i].name);
        fprintf(file, "%d\n", products[i].quantity);
        fprintf(file, "%d\n", products[i].workshop);

        // Save product code
        if (products[i].code.int_code != 0) {
            fprintf(file, "%d\n", products[i].code.int_code);
        }
        else {
            fprintf(file, "%s\n", products[i].code.str_code);
        }
    }

    fclose(file);
}

// Load from file
void load_from_file() {
    FILE* file = fopen("products.txt", "r");
    if (!file) {
        printf("File not found. New file will be created.\n");
        return;
    }

    free_memory(); // Free old data

    fscanf(file, "%d", &product_count);
    clear_input_buffer();
    capacity = product_count;

    products = (Product*)malloc(capacity * sizeof(Product));
    if (!products) {
        printf("Memory allocation error!\n");
        exit(1);
    }

    char buffer[100];
    for (int i = 0; i < product_count; i++) {
        // Read name (can contain spaces)
        fgets(buffer, sizeof(buffer), file);
        buffer[strcspn(buffer, "\n")] = 0;
        strcpy(products[i].name, buffer);

        // Read quantity
        fgets(buffer, sizeof(buffer), file);
        products[i].quantity = atoi(buffer);

        // Read workshop number
        fgets(buffer, sizeof(buffer), file);
        products[i].workshop = atoi(buffer);

        // Read product code
        fgets(buffer, sizeof(buffer), file);
        buffer[strcspn(buffer, "\n")] = 0;

        // Check if code is a number
        int is_number = 1;
        for (int j = 0; buffer[j] != '\0'; j++) {
            if (buffer[j] < '0' || buffer[j] > '9') {
                is_number = 0;
                break;
            }
        }

        if (is_number) {
            products[i].code.int_code = atoi(buffer);
            products[i].code.str_code[0] = '\0';
        }
        else {
            strcpy(products[i].code.str_code, buffer);
            products[i].code.int_code = 0;
        }
    }

    fclose(file);
    printf("Data loaded from file products.txt\n");
}

// Edit records in file
void edit_in_file() {
    printf("\nEditing data in file:\n");
    printf("1. Edit specific record\n");
    printf("2. Add new record to file\n");
    printf("Select action: ");

    int choice;
    scanf("%d", &choice);
    clear_input_buffer();

    if (choice == 1) {
        edit_product(); // Use existing function
        save_to_file(); // Save changes to file
        printf("Changes saved to file!\n");
    }
    else if (choice == 2) {
        add_product(); // Use existing function
        save_to_file(); // Save changes to file
        printf("New record added to file!\n");
    }
    else {
        printf("Invalid choice!\n");
    }
}

// Free memory
void free_memory() {
    if (products) {
        free(products);
        products = NULL;
    }
    product_count = 0;
    capacity = 0;
}