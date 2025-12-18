#include <stdio.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif

#define MAX_STUDENTS 100
#define NAME_LEN 100

typedef struct {
    int id;
    char name[NAME_LEN];
    int age;
    float gpa;
} Student;

/* ========== HÀM HIỂN THỊ ========== */

static void printHeader(void) {
    printf("+------+------------------------------+-----+-----+\n");
    printf("| %-4s | %-28s | %-3s | %-3s |\n", "ID", "Name", "Age", "GPA");
    printf("+------+------------------------------+-----+-----+\n");
}

static void printRow(const Student *s) {
    printf("| %-4d | %-28s | %-3d | %-3.2f |\n", s->id, s->name, s->age, s->gpa);
}

static void printFooter(void) {
    printf("+------+------------------------------+-----+-----+\n");
}

static void displayStudentList(const Student *students, int count) {
    if (count <= 0) {
        printf("\nDanh sách sinh viên rỗng.\n");
        return;
    }
    printf("\n");
    printHeader();
    for (int i = 0; i < count; i++) {
        printRow(&students[i]);
    }
    printFooter();
}

static void displayMenu(void) {
    printf("\n");
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║     CHƯƠNG TRÌNH QUẢN LÝ DANH SÁCH SINH VIÊN  ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
    printf("\n1. Nhập sinh viên từ bàn phím\n");
    printf("2. In danh sách sinh viên\n");
    printf("3. Ghi danh sách vào file (students.txt)\n");
    printf("4. Đọc danh sách từ file lớp (classA/B/C.txt)\n");
    printf("5. Tìm kiếm sinh viên theo ID\n");
    printf("6. Sắp xếp sinh viên theo GPA giảm dần\n");
    printf("7. Thêm sinh viên mới\n");
    printf("8. Xóa sinh viên\n");
    printf("9. Tính GPA trung bình lớp\n");
    printf("0. Thoát chương trình\n");
    printf("\nChọn chức năng (0-9): ");
}

/* ========== HÀM FILE I/O ========== */

static int saveToFile(const Student *students, int n) {
    FILE *f = fopen("d:\\ss8\\students.txt", "w");
    if (!f) {
        fprintf(stderr, "Loi: Khong the tao/mo file students.txt.\n");
        return 0;
    }

    fprintf(f, "+------+------------------------------+-----+-----+\n");
    fprintf(f, "| %-4s | %-28s | %-3s | %-3s |\n", "ID", "Name", "Age", "GPA");
    fprintf(f, "+------+------------------------------+-----+-----+\n");
    for (int i = 0; i < n; i++) {
        fprintf(f, "| %-4d | %-28s | %-3d | %-3.2f |\n",
                students[i].id, students[i].name, students[i].age, students[i].gpa);
    }
    fprintf(f, "+------+------------------------------+-----+-----+\n");
    fclose(f);
    return 1;
}

static const char* promptClassFilename(void) {
    static char filename[100];
    printf("\n=== CHỌN FILE LỚP ===\n");
    printf("Sẵn có: classA.txt, classB.txt, classC.txt\n");
    printf("Nhập tên file (ví dụ: classA.txt): ");
    if (fgets(filename, sizeof(filename), stdin) == NULL) {
        return NULL;
    }
    size_t len = strlen(filename);
    if (len > 0 && filename[len - 1] == '\n') {
        filename[len - 1] = '\0';
    }
    return filename;
}

/*
 * readStudentDataFromFile - Đọc dữ liệu sinh viên từ file
 * Path: đường dẫn file (VD: "d:\\ss8\\classA.txt")
 * Format: id;name;age;gpa
 * Trả về: số lượng sinh viên đọc được hoặc -1 nếu lỗi
 */
static int readStudentDataFromFile(const char *path, Student *students, int max) {
    FILE *f = fopen(path, "r");
    if (!f) {
        fprintf(stderr, "Loi: Khong the mo file '%s'.\n", path);
        return -1;
    }

    char line[256];
    int count = 0;

    while (fgets(line, sizeof(line), f) != NULL && count < max) {
        int id = 0, age = 0;
        float gpa = 0.0f;
        char namebuf[NAME_LEN] = {0};

        size_t L = strlen(line);
        if (L > 0 && line[L - 1] == '\n') {
            line[L - 1] = '\0';
        }

        int matched = sscanf(line, "%d;%99[^;];%d;%f", &id, namebuf, &age, &gpa);
        if (matched == 4) {
            students[count].id = id;
            strncpy(students[count].name, namebuf, NAME_LEN - 1);
            students[count].name[NAME_LEN - 1] = '\0';
            students[count].age = age;
            students[count].gpa = gpa;
            count++;
        }
    }

    fclose(f);
    printf("✓ Đọc thành công %d sinh viên từ file '%s'.\n", count, path);
    return count;
}

/* ========== HÀM TÍNH TOÁN ========== */

static float calculateAverageGPA(const Student *students, int count) {
    if (count <= 0) return 0.0f;
    
    float sum = 0.0f;
    for (int i = 0; i < count; i++) {
        sum += students[i].gpa;
    }
    return sum / count;
}

static void printAverageGPA(const Student *students, int count) {
    if (count <= 0) {
        printf("\nThông báo: Danh sách sinh viên rỗng.\n");
        return;
    }
    float avg = calculateAverageGPA(students, count);
    printf("\nGPA trung bình của lớp: %.2f\n", avg);
}

/* ========== HÀM SẮPXẾP ========== */

static void sortStudentsByGPADesc(Student *students, int count) {
    if (count <= 1) return;
    
    for (int i = 0; i < count - 1; i++) {
        int maxIdx = i;
        for (int j = i + 1; j < count; j++) {
            if (students[j].gpa > students[maxIdx].gpa) {
                maxIdx = j;
            }
        }
        if (maxIdx != i) {
            Student temp = students[i];
            students[i] = students[maxIdx];
            students[maxIdx] = temp;
        }
    }
}

static void sortAndDisplay(Student *students, int count) {
    if (count <= 0) {
        printf("\nDanh sách sinh viên rỗng, không thể sắp xếp.\n");
        return;
    }
    
    Student tempList[MAX_STUDENTS];
    for (int i = 0; i < count; i++) {
        tempList[i] = students[i];
    }
    
    printf("\nDanh sách trước sắp xếp:\n");
    displayStudentList(tempList, count);
    
    sortStudentsByGPADesc(tempList, count);
    
    printf("\nDanh sách sau sắp xếp (GPA giảm dần):\n");
    displayStudentList(tempList, count);
}

/* ========== HÀM TÌM KIẾM ========== */

static int searchStudentByID(const Student *students, int count, int id) {
    if (id < 0) return -1;
    for (int i = 0; i < count; i++) {
        if (students[i].id == id) return i;
    }
    return -1;
}

static int promptSearchID(void) {
    int id;
    printf("\nNhập ID cần tìm: ");
    if (scanf("%d", &id) != 1) {
        while (getchar() != '\n');
        return -1;
    }
    while (getchar() != '\n');
    return id;
}

static void findStudentInteractive(const Student *students, int count) {
    int searchId = promptSearchID();
    if (searchId >= 0) {
        int found = searchStudentByID(students, count, searchId);
        printf("\nKết quả tìm kiếm:\n");
        if (found >= 0) {
            printf("✓ Tìm thấy:\n");
            printf("  ID: %d | Tên: %s | Tuổi: %d | GPA: %.2f\n",
                   students[found].id, students[found].name, students[found].age, students[found].gpa);
        } else {
            printf("✗ Không tìm thấy sinh viên với ID %d.\n", searchId);
        }
    } else {
        printf("ID không hợp lệ.\n");
    }
}

/* ========== HÀM THÊM/XÓA ========== */

static int addStudentByID(Student *students, int *count, const Student *newStudent, int maxSize) {
    if (count == NULL || newStudent == NULL) return -2;
    
    /* Kiểm tra ID trùng */
    for (int i = 0; i < *count; i++) {
        if (students[i].id == newStudent->id) {
            fprintf(stderr, "Loi: ID %d da ton tai trong danh sach.\n", newStudent->id);
            return -1;
        }
    }
    
    /* Kiểm tra dung lượng */
    if (*count >= maxSize) {
        fprintf(stderr, "Loi: Danh sach da day (toi da %d sinh vien).\n", maxSize);
        return -2;
    }
    
    /* Thêm sinh viên mới vào cuối danh sách */
    students[*count] = *newStudent;
    (*count)++;
    printf("✓ Them sinh vien ID %d thanh cong.\n", newStudent->id);
    return 1;
}

static int deleteStudentByID(Student *students, int *count, int idToDelete) {
    if (count == NULL) return -2;
    
    /* Tìm vị trí sinh viên cần xóa */
    int foundIdx = -1;
    for (int i = 0; i < *count; i++) {
        if (students[i].id == idToDelete) {
            foundIdx = i;
            break;
        }
    }
    
    /* Nếu không tìm thấy */
    if (foundIdx == -1) {
        fprintf(stderr, "Loi: ID %d khong ton tai trong danh sach.\n", idToDelete);
        return -1;
    }
    
    /* Dịch chuyển các phần tử sau lên trước */
    for (int i = foundIdx; i < *count - 1; i++) {
        students[i] = students[i + 1];
    }
    
    (*count)--;
    printf("✓ Xoa sinh vien ID %d thanh cong.\n", idToDelete);
    return 1;
}

/* ========== HÀM NHẬP LIỆU ========== */

static void inputNewStudent(Student *students, int *count, int maxSize) {
    if (count == NULL || *count >= maxSize) {
        fprintf(stderr, "Loi: Danh sach day hoac tham so khong hop le.\n");
        return;
    }
    
    Student newStudent;
    printf("\n--- Nhập thông tin sinh viên mới ---\n");
    
    printf("ID: ");
    if (scanf("%d", &newStudent.id) != 1) {
        fprintf(stderr, "Loi: ID khong hop le.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');
    
    printf("Tên: ");
    if (fgets(newStudent.name, NAME_LEN, stdin) == NULL) {
        newStudent.name[0] = '\0';
    } else {
        size_t len = strlen(newStudent.name);
        if (len > 0 && newStudent.name[len - 1] == '\n')
            newStudent.name[len - 1] = '\0';
    }
    
    printf("Tuổi: ");
    if (scanf("%d", &newStudent.age) != 1) {
        fprintf(stderr, "Loi: Tuoi khong hop le.\n");
        while (getchar() != '\n');
        return;
    }
    
    printf("GPA: ");
    if (scanf("%f", &newStudent.gpa) != 1) {
        fprintf(stderr, "Loi: GPA khong hop le.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');
    
    addStudentByID(students, count, &newStudent, maxSize);
}

static void deleteStudentInteractive(Student *students, int *count) {
    if (count == NULL) return;
    
    printf("\nNhập ID sinh viên cần xóa: ");
    int idToDelete;
    if (scanf("%d", &idToDelete) != 1) {
        fprintf(stderr, "Loi: ID khong hop le.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');
    
    deleteStudentByID(students, count, idToDelete);
}

static void loadStudentFromFile(Student *students, int *count, int maxSize) {
    if (count == NULL) return;
    
    const char *classFile = promptClassFilename();
    if (classFile == NULL || strlen(classFile) == 0) {
        fprintf(stderr, "Tên file không hợp lệ.\n");
        return;
    }
    
    char fullPath[256];
    snprintf(fullPath, sizeof(fullPath), "d:\\ss8\\%s", classFile);
    
    int m = readStudentDataFromFile(fullPath, students, maxSize);
    if (m >= 0) {
        *count = m;
    }
}

/* ========== HÀM KHỞI TẠO ========== */

static void initializeSampleStudents(Student *students, int *maxCount) {
    if (maxCount == NULL) return;
    
    students[0] = (Student){1001, "Nguyen Van An", 20, 8.5f};
    students[1] = (Student){1002, "Tran Thi Binh", 19, 9.0f};
    students[2] = (Student){1003, "Le Quang Cuong", 21, 7.5f};
    students[3] = (Student){1004, "Pham Thi Duc", 20, 8.2f};
    students[4] = (Student){1005, "Hoang Van Em", 22, 7.8f};
    students[5] = (Student){1006, "Vu Thi Phuong", 19, 8.8f};
    students[6] = (Student){1007, "Dang Van Giang", 21, 6.5f};
    students[7] = (Student){1008, "Bui Thi Huong", 20, 8.3f};
    students[8] = (Student){1009, "Trinh Van Khanh", 19, 9.2f};
    students[9] = (Student){1010, "Nguyen Thi Linh", 22, 7.0f};
    
    *maxCount = 10;
    printf("✓ Đã khởi tạo 10 sinh viên mẫu.\n");
}

/* ========== HÀM CHÍNH ========== */

int main(void) {
    Student students[MAX_STUDENTS];
    int count = 0;
    /* Trên Windows: đặt code page sang UTF-8 để hiển thị tiếng Việt và ký tự khung */
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    printf("╔════════════════════════════════════════════════╗\n");
    printf("║   HỆ THỐNG QUẢN LÝ DANH SÁCH SINH VIÊN v2.0   ║\n");
    printf("╚════════════════════════════════════════════════╝\n");
    
    initializeSampleStudents(students, &count);
    
    int choice = -1;
    while (choice != 0) {
        displayMenu();
        
        if (scanf("%d", &choice) != 1) {
            fprintf(stderr, "Loi: Lua chon khong hop le.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');
        
        switch (choice) {
            case 1:
                inputNewStudent(students, &count, MAX_STUDENTS);
                break;
            case 2:
                displayStudentList(students, count);
                break;
            case 3:
                if (saveToFile(students, count)) {
                    printf("✓ Danh sách đã được lưu vào students.txt.\n");
                } else {
                    fprintf(stderr, "Loi: Khong the luu danh sach.\n");
                }
                break;
            case 4:
                loadStudentFromFile(students, &count, MAX_STUDENTS);
                break;
            case 5:
                findStudentInteractive(students, count);
                break;
            case 6:
                sortAndDisplay(students, count);
                break;
            case 7:
                inputNewStudent(students, &count, MAX_STUDENTS);
                break;
            case 8:
                deleteStudentInteractive(students, &count);
                break;
            case 9:
                printAverageGPA(students, count);
                break;
            case 0:
                printf("\n");
                printf("╔════════════════════════════════════════════════╗\n");
                printf("║         Cảm ơn đã sử dụng chương trình!         ║\n");
                printf("║             Tạm biệt và hẹn gặp lại!            ║\n");
                printf("╚════════════════════════════════════════════════╝\n");
                break;
            default:
                fprintf(stderr, "Loi: Lua chon khong hop le. Vui long chon 0-9.\n");
                break;
        }
    }
    
    return 0;
}
