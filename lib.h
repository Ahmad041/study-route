#ifndef LIB_H
#define LIB_H

#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// Definisi Key Mapping
#define ENTER 13
#define ESC 27
#define BACKSPACE 8
#define SPACE 32
#define UP_ARROW 72
#define DOWN_ARROW 80
#define LEFT_ARROW 75
#define RIGHT_ARROW 77

// Definisi Warna agar mudah diingat
enum Colors {
    BLACK = 0, BLUE = 1, GREEN = 2, AQUA = 3, RED = 4, PURPLE = 5, YELLOW = 14, WHITE = 15, GRAY = 8
};

// ==========================================
// 1. SYSTEM UTILITIES (Cursor, Screen, Size)
// ==========================================


// Mendapatkan posisi X kursor saat ini
int wherex() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if(GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
        return csbi.dwCursorPosition.X;
    return -1;
}

// Mendapatkan posisi Y kursor saat ini
int wherey() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if(GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
        return csbi.dwCursorPosition.Y;
    return -1;
}

void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}

void showCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = TRUE;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void setColor(int text, int bg) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (bg * 16) + text);
}

int getConsoleWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

int getConsoleHeight() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

void fullscreen(){
    ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
    // Simulasi Alt+Enter (Mungkin tidak jalan di Windows 11 Terminal baru, tapi oke untuk Legacy Console)
    keybd_event(VK_MENU,0x42,0,0);
    keybd_event(VK_RETURN,0x1c,0,0);
    keybd_event(VK_RETURN,0x1c,KEYEVENTF_KEYUP,0);
    keybd_event(VK_MENU,0x42,KEYEVENTF_KEYUP,0);
}

void remove_scrollbar() {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(handle, &info);
    COORD new_size = {
        (SHORT)(info.srWindow.Right - info.srWindow.Left + 1),
        (SHORT)(info.srWindow.Bottom - info.srWindow.Top + 1)
    };
    SetConsoleScreenBufferSize(handle, new_size);
}

// ==========================================
// 2. OUTPUT & DRAWING (Text, Borders)
// ==========================================

// Menampilkan karakter di posisi tertentu
void gotoPrintChar(int x, int y, int dec){
    gotoxy(x,y);
    printf("%c",dec);
}

// Menampilkan string di posisi tertentu
void gotoPrintString(int x, int y, char *text){
    gotoxy(x,y);
    printf("%s", text);
}

// [UPGRADE] Menampilkan teks rata tengah pada baris Y tertentu
void tengah(int y, char *text) {
    int width = getConsoleWidth();
    int len = strlen(text);
    int x = (width - len) / 2;
    gotoxy(x, y);
    printf("%s", text);
}

// Membersihkan area tertentu
void clearArea(int x, int y, int width, int height) {
    char *spaces = (char*)malloc(width + 1);
    memset(spaces, ' ', width);
    spaces[width] = '\0';

    for (int i = 0; i < height; i++) {
        gotoxy(x, y + i);
        printf("%s", spaces);
    }
    free(spaces);
}

void borderDoubleLine(int x,int y,int lebar, int tinggi) {
    // Atas
    gotoxy(x, y); printf("%c", 201);
    for(int i=0; i<lebar; i++) printf("%c", 205);
    printf("%c", 187);

    // Samping
    for(int i=1; i<=tinggi; i++) {
        gotoxy(x, y+i); printf("%c", 186);
        gotoxy(x+lebar+1, y+i); printf("%c", 186);
    }

    // Bawah
    gotoxy(x, y+tinggi+1); printf("%c", 200);
    for(int i=0; i<lebar; i++) printf("%c", 205);
    printf("%c", 188);
}

void borderLine(int x,int y,int lebar, int tinggi) {
    // Atas
    gotoxy(x, y); printf("%c", 218);
    for(int i=0; i<lebar; i++) printf("%c", 196);
    printf("%c", 191);

    // Samping
    for(int i=1; i<=tinggi; i++) {
        gotoxy(x, y+i); printf("%c", 179);
        gotoxy(x+lebar+1, y+i); printf("%c", 179);
    }

    // Bawah
    gotoxy(x, y+tinggi+1); printf("%c", 192);
    for(int i=0; i<lebar; i++) printf("%c", 196);
    printf("%c", 217);
}

// ==========================================
// 3. UI COMPONENTS (Loading, MsgBox, Menu)
// ==========================================

// [UPGRADE] Loading Bar Animation
void loadingBar(int x, int y, int duration_ms) {
    hideCursor();
    gotoxy(x, y); printf("Loading: [");
    gotoxy(x + 20, y); printf("]");

    for(int i = 0; i < 19; i++) {
        gotoxy(x + 10, y + 1); // Persentase di bawah bar
        printf("%d %%", (i + 1) * 5);

        gotoxy(x + 1 + i, y);
        setColor(GREEN, 0); // Warna hijau untuk bar
        printf("%c", 219);  // Karakter block
        setColor(WHITE, 0); // Reset warna

        Sleep(duration_ms / 20);
    }
    gotoxy(x, y+1); printf("                 "); // Hapus persentase
    showCursor();
}

// [UPGRADE] Message Box Popup
void msgBox(char *title, char *message, int color) {
    int width = 40;
    int height = 5;
    int screenW = getConsoleWidth();
    int screenH = getConsoleHeight();
    int x = (screenW - width) / 2;
    int y = (screenH - height) / 2;

    clearArea(x, y, width + 2, height + 2);

    // Simpan area belakang (advance technique, tapi untuk simpel kita clear saja nanti)
    setColor(color, 0);
    borderDoubleLine(x, y, width, height);

    // Title
    gotoxy(x + 2, y); printf(" %s ", title);

    // Message (Center in box)
    int msgX = x + (width - strlen(message))/2;
    gotoxy(msgX, y + 2);
    printf("%s", message);

    gotoxy(x + (width-18)/2, y + 4);
    setColor(color + 8, 0); // Warna lebih terang untuk instruksi
    printf(" Press ENTER to close ");
    setColor(WHITE, 0);

    // Wait
    while(getch() != ENTER);

    // Clear popup area after close
    clearArea(x, y, width + 2, height + 2);
}

// [UPGRADE] Menu Interaktif (Return index pilihan: 0, 1, 2...)
int drawMenu(int x, int y, char options[][50], int totalOption) {
    int selected = 0;
    int key;

    hideCursor();
    while(1) {
        for(int i = 0; i < totalOption; i++) {
            gotoxy(x, y + i);
            if(i == selected) {
                setColor(BLACK, WHITE); // Highlight (Background Putih, Teks Hitam)
                printf(" > %-20s ", options[i]); // Padding 20 char
            } else {
                setColor(WHITE, 0);     // Normal
                printf("   %-20s ", options[i]);
            }
            setColor(WHITE, 0); // Reset
        }

        key = getch();

        if(key == UP_ARROW) {
            printf("\a");
            selected--;
            if(selected < 0) selected = totalOption - 1;
        } else if(key == DOWN_ARROW) {
            selected++;
            if(selected >= totalOption) selected = 0;
        } else if(key == ENTER) {
            return selected; // Mengembalikan index menu yang dipilih
        }
    }
}

// ==========================================
// 4. INPUT HANDLING
// ==========================================

void scanInputString(int x, int y, char text[100], int maxChar) {
    char ch;
    int i = 0;
    gotoxy(x, y);
    showCursor();
    text[0] = '\0'; // Inisialisasi

    while (1) {
        ch = getch();
        if (ch == ENTER) {
            text[i] = '\0';
            break;
        } else if (ch == BACKSPACE) {
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else if (i < maxChar && isprint(ch)) {
            text[i] = ch;
            i++;
            printf("%c", ch);
        }
    }
}

void PrintTxt(int x,int y,char txtFile[100]) {
    char text[255];
    int penambah = 0;

    FILE *file = fopen(txtFile, "r");

    while (fgets(text, sizeof(text), file)) {
        gotoxy(x,y+penambah);
        printf("%s", text);
        penambah++;
    }
    fclose(file);
}

void scanInputPassword(int x, int y, char text[100], int maxChar) {
    char ch;
    int i = 0;
    gotoxy(x, y);
    showCursor();

    while (1) {
        ch = getch();
        if (ch == ENTER) {
            text[i] = '\0';
            break;
        } else if (ch == BACKSPACE) {
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else if (i < maxChar && isprint(ch)) {
            text[i] = ch;
            i++;
            printf("*"); // Masking character
        }
    }
}

void scanInputMoney(int x, int y, int *nilai, int maxDigit) {
    char input[50] = "";
    char ch;
    int i = 0;

    gotoxy(x, y);
    showCursor();

    while(1) {
        ch = getch();

        if (ch == ENTER) {
             if (i > 0) break; // Harus ada input
        }
        else if (ch == BACKSPACE) {
            if (i > 0) {
                i--;
                input[i] = '\0';
                // Clear baris dan print ulang format rupiah
                gotoxy(x, y); printf("                    "); // Hapus bekas
                gotoxy(x, y);

                // Print ulang dengan format (logika sederhana)
                // Note: Untuk project besar, gunakan fungsi format rupiah terpisah
                printf("%s", input);
            }
        }
        else if (isdigit(ch) && i < maxDigit) {
            input[i] = ch;
            input[i+1] = '\0';
            i++;
            gotoxy(x, y);
            printf("%s", input);
        }
    }
    *nilai = atoi(input);
}

// Fungsi Font (Hanya bekerja jika Console mendukung font changing)
void setConsoleFont(int fontSize, const wchar_t* fontName) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX fontInfo;
    fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    GetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);
    fontInfo.dwFontSize.X = 0;
    fontInfo.dwFontSize.Y = fontSize;
    wcscpy(fontInfo.FaceName, fontName);
    SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);
}

// [NEW] Membuat bayangan solid untuk efek 3D
void drawShadow(int x, int y, int width, int height) {
    setColor(BLACK, GRAY); // Warna bayangan (Teks Hitam, Background Abu)

    // Bayangan Bawah
    for (int i = 1; i <= width; i++) {
        gotoxy(x + i, y + height + 1);
        printf("%c", 176); // Karakter raster (arsiran)
    }

    // Bayangan Samping Kanan
    for (int i = 1; i <= height + 1; i++) {
        gotoxy(x + width + 1, y + i);
        printf("%c", 176);
    }
    setColor(WHITE, BLACK); // Reset warna default
}

// [NEW] Wrapper untuk menggambar Kotak dengan Bayangan sekaligus
void drawBoxWithShadow(int x, int y, int width, int height, char *title) {
    drawShadow(x, y, width, height);
    setColor(WHITE, BLACK);

    clearArea(x, y, width + 1, height + 1);

    borderDoubleLine(x, y, width, height);

    if(strlen(title) > 0){
        int textX = x + (width - strlen(title)) / 2;
        gotoxy(textX, y);
        printf(" %s ", title);
    }
}

// ==========================================
// 5. ANIMATION TOOLS (New Upgrades)
// ==========================================

// Efek teks muncul satu per satu seperti diketik (Cocok untuk intro game/cerita)
void animateTyping(int x, int y, char *text, int speed_ms) {
    gotoxy(x, y);
    for(int i = 0; text[i] != '\0'; i++) {
        printf("%c", text[i]);
        Sleep(speed_ms); // Menggunakan S besar (Milidetik)
        
        // Cek jika user menekan tombol apapun untuk skip animasi
        if(kbhit()) {
            getch(); // Buang karakter input
            speed_ms = 0; // Percepat jadi instan
        }
    }
}

// Animasi Loading berputar sederhana di satu titik
void animateSpinner(int x, int y, int loops, int speed_ms) {
    char spinChars[] = {'|', '/', '-', '\\'};
    hideCursor();
    for(int i = 0; i < loops * 4; i++) { // loops dikali 4 karena ada 4 frame
        gotoxy(x, y);
        setColor(AQUA, BLACK);
        printf("%c", spinChars[i % 4]);
        Sleep(speed_ms);
    }
    setColor(WHITE, BLACK);
    gotoxy(x, y); printf(" "); // Bersihkan setelah selesai
    showCursor();
}

// Animasi teks berkedip (misal: "Press Any Key")
void animateBlinkText(int x, int y, char *text, int times) {
    hideCursor();
    for(int i = 0; i < times; i++) {
        gotoxy(x, y); printf("%s", text); // Muncul
        Sleep(500);
        gotoxy(x, y); 
        for(int j=0; j<strlen(text); j++) printf(" "); // Hilang (timpa spasi)
        Sleep(300);
    }
    gotoxy(x, y); printf("%s", text); // Pastikan teks tetap ada di akhir
    showCursor();
}

#endif