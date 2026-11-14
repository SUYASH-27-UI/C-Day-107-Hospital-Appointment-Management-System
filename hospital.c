
#include <stdio.h>
#include <string.h>

#define MAX 100
#define FILE_NAME "appointments.txt"

struct Appointment {
    int id;
    char patient[50];
    char doctor[50];
    char date[20];
    char status[20];
};

void saveAppointments(struct Appointment a[], int count) {
    FILE *fp = fopen(FILE_NAME, "w");

    if (fp == NULL) {
        printf("Error saving appointments!\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d|%s|%s|%s|%s\n",
                a[i].id, a[i].patient, a[i].doctor,
                a[i].date, a[i].status);
    }

    fclose(fp);
}

int loadAppointments(struct Appointment a[]) {
    FILE *fp = fopen(FILE_NAME, "r");

    if (fp == NULL) return 0;

    int count = 0;

    while (count < MAX &&
           fscanf(fp, "%d|%49[^|]|%49[^|]|%19[^|]|%19[^\n]",
                  &a[count].id,
                  a[count].patient,
                  a[count].doctor,
                  a[count].date,
                  a[count].status) == 5) {
        count++;
    }

    fclose(fp);
    return count;
}

int findAppointment(struct Appointment a[], int count, int id) {
    for (int i = 0; i < count; i++) {
        if (a[i].id == id) return i;
    }

    return -1;
}

int generateId(struct Appointment a[], int count) {
    int maxId = 1000;

    for (int i = 0; i < count; i++) {
        if (a[i].id > maxId) maxId = a[i].id;
    }

    return maxId + 1;
}

void bookAppointment(struct Appointment a[], int *count) {
    if (*count >= MAX) {
        printf("Appointment limit reached!\n");
        return;
    }

    int id = generateId(a, *count);

    a[*count].id = id;

    printf("Enter Patient Name: ");
    scanf(" %49[^\n]", a[*count].patient);

    printf("Enter Doctor Name: ");
    scanf(" %49[^\n]", a[*count].doctor);

    printf("Enter Date (DD-MM-YYYY): ");
    scanf(" %19s", a[*count].date);

    strcpy(a[*count].status, "Scheduled");

    (*count)++;
    saveAppointments(a, *count);

    printf("Appointment booked successfully!\n");
    printf("Appointment ID: %d\n", id);
}

void viewAppointments(struct Appointment a[], int count) {
    if (count == 0) {
        printf("No appointments available.\n");
        return;
    }

    printf("\n--- Appointment Records ---\n");

    for (int i = 0; i < count; i++) {
        printf("\nID: %d\n", a[i].id);
        printf("Patient: %s\n", a[i].patient);
        printf("Doctor: %s\n", a[i].doctor);
        printf("Date: %s\n", a[i].date);
        printf("Status: %s\n", a[i].status);
    }
}

void searchAppointment(struct Appointment a[], int count) {
    int id;

    printf("Enter Appointment ID: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID!\n");
        return;
    }

    int index = findAppointment(a, count, id);

    if (index == -1) {
        printf("Appointment not found.\n");
        return;
    }

    printf("\nPatient: %s\n", a[index].patient);
    printf("Doctor: %s\n", a[index].doctor);
    printf("Date: %s\n", a[index].date);
    printf("Status: %s\n", a[index].status);
}

void updateStatus(struct Appointment a[], int count) {
    int id;

    printf("Enter Appointment ID: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID!\n");
        return;
    }

    int index = findAppointment(a, count, id);

    if (index == -1) {
        printf("Appointment not found.\n");
        return;
    }

    printf("1. Scheduled\n");
    printf("2. Completed\n");
    printf("3. Cancelled\n");
    printf("Choose New Status: ");

    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input!\n");
        return;
    }

    if (choice == 1) {
        strcpy(a[index].status, "Scheduled");
    } else if (choice == 2) {
        strcpy(a[index].status, "Completed");
    } else if (choice == 3) {
        strcpy(a[index].status, "Cancelled");
    } else {
        printf("Invalid status choice!\n");
        return;
    }

    saveAppointments(a, count);
    printf("Appointment status updated!\n");
}

void cancelAppointment(struct Appointment a[], int *count) {
    int id;

    printf("Enter Appointment ID to Cancel: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID!\n");
        return;
    }

    int index = findAppointment(a, *count, id);

    if (index == -1) {
        printf("Appointment not found.\n");
        return;
    }

    for (int i = index; i < *count - 1; i++) {
        a[i] = a[i + 1];
    }

    (*count)--;
    saveAppointments(a, *count);

    printf("Appointment deleted successfully!\n");
}

int main() {
    struct Appointment appointments[MAX];
    int count = loadAppointments(appointments);
    int choice;

    while (1) {
        printf("\n===== HOSPITAL APPOINTMENT SYSTEM =====\n");
        printf("1. Book Appointment\n");
        printf("2. View Appointments\n");
        printf("3. Search Appointment\n");
        printf("4. Update Status\n");
        printf("5. Cancel Appointment\n");
        printf("6. Exit\n");
        printf("Enter Choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Restart the program.\n");
            break;
        }

        switch (choice) {
            case 1:
                bookAppointment(appointments, &count);
                break;
            case 2:
                viewAppointments(appointments, count);
                break;
            case 3:
                searchAppointment(appointments, count);
                break;
            case 4:
                updateStatus(appointments, count);
                break;
            case 5:
                cancelAppointment(appointments, &count);
                break;
            case 6:
                printf("Hospital System Closed.\n");
                return 0;
            default:
                printf("Invalid choice!\n");
        }
    }

    return 0;
}
