#include <iostream>
#include <string>
#include <queue>
#include <iomanip>

using namespace std;

struct Film {
    string judul;
    int tahunRilis;
    string genre;
    string* aktor[3];
};

struct User {
    string username;
    string password;
};

User users[] = {{"fahmi", "123"}, {"panji", "123"},{"marcel","123"}};
queue<Film> watchlist;

bool login(string username, string password) {
    int userCount = sizeof(users) / sizeof(users[0]);
    for (int i = 0; i < userCount; ++i) {
        if (users[i].username == username && users[i].password == password) {
            return true;
        }
    }
    return false;
}

void tambahFilm() {
    Film film;
    cout << "\n=== Tambah Film ===\n";
    cout << "Masukkan judul film: ";
    cin.ignore();
    getline(cin, film.judul);
    cout << "Masukkan tahun rilis: ";
    cin >> film.tahunRilis;
    cin.ignore(); 
    cout << "Masukkan genre: ";
    getline(cin, film.genre);
    for (int i = 0; i < 3; ++i) {
        film.aktor[i] = new string;
        cout << "Masukkan aktor " << i + 1 << ": ";
        getline(cin, *film.aktor[i]);
    }
    watchlist.push(film);
    cout << "Film berhasil ditambahkan ke watchlist.\n";
}

void hapusFilm() {
    if (!watchlist.empty()) {
        Film film = watchlist.front();
        for (int i = 0; i < 3; ++i) {
            delete film.aktor[i]; 
        }
        watchlist.pop();
        cout << "Film berhasil dihapus dari watchlist.\n";
    } else {
        cout << "Watchlist kosong.\n";
    }
}

void hapusFilmBerdasarkanJudul() {
    string judul;
    cout << "\n=== Hapus Film Berdasarkan Judul ===\n";
    cout << "Masukkan judul film yang ingin dihapus: ";
    cin.ignore();
    getline(cin, judul);
    queue<Film> tempQueue;
    bool found = false;
    while (!watchlist.empty()) {
        Film film = watchlist.front();
        watchlist.pop();
        if (film.judul == judul) {
            found = true;
            for (int i = 0; i < 3; ++i) {
                delete film.aktor[i];
            }
            cout << "Film dengan judul \"" << judul << "\" berhasil dihapus.\n";
        } else {
            tempQueue.push(film);
        }
    }
    watchlist = tempQueue;
    if (!found) {
        cout << "Film dengan judul \"" << judul << "\" tidak ditemukan.\n";
    }
}

void hapusSemuaFilm() {
    while (!watchlist.empty()) {
        Film film = watchlist.front();
        for (int i = 0; i < 3; ++i) {
            delete film.aktor[i]; 
        }
        watchlist.pop();
    }
    cout << "Semua film berhasil dihapus dari watchlist.\n";
}

void tampilkanWatchlist() {
    const int maxFilms = 100;
    Film sortedWatchlist[maxFilms];
    int count = 0;
    queue<Film> tempQueue = watchlist;
    while (!tempQueue.empty() && count < maxFilms) {
        sortedWatchlist[count++] = tempQueue.front();
        tempQueue.pop();
    }
    for (int i = 0; i < count - 1; ++i) {
        for (int j = 0; j < count - i - 1; ++j) {
            if (sortedWatchlist[j].tahunRilis > sortedWatchlist[j + 1].tahunRilis) {
                swap(sortedWatchlist[j], sortedWatchlist[j + 1]);
            }
        }
    }
    cout << "\n=== Watchlist ===\n";
    cout << left << setw(30) << "Judul" << setw(15) << "Tahun Rilis" << setw(20) << "Genre" << setw(30) << "Aktor" << endl;
    cout << "---------------------------------------------------------------------------------------------\n";
    for (int i = 0; i < count; ++i) {
        cout << left << setw(30) << sortedWatchlist[i].judul << setw(15) << sortedWatchlist[i].tahunRilis << setw(20) << sortedWatchlist[i].genre;
        for (int j = 0; j < 3; ++j) {
            cout << *sortedWatchlist[i].aktor[j];
            if (j < 2) cout << ", ";
        }
        cout << endl;
    }
}

void cariFilmBerdasarkanJudul() {
    string judul;
    cout << "\n=== Cari Film Berdasarkan Judul ===\n";
    cout << "Masukkan judul film yang dicari: ";
    cin.ignore();
    getline(cin, judul);
    bool found = false;
    queue<Film> tempQueue = watchlist;
    while (!tempQueue.empty()) {
        Film film = tempQueue.front();
        tempQueue.pop();
        if (film.judul == judul) {
            cout << "Film ditemukan: Judul: " << film.judul << ", Tahun Rilis: " << film.tahunRilis << ", Genre: " << film.genre << ", Aktor: ";
            for (int i = 0; i < 3; ++i) {
                cout << *film.aktor[i];
                if (i < 2) cout << ", ";
            }
            cout << endl;
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "Film tidak ditemukan.\n";
    }
}

void cariFilmBerdasarkanTahun() {
    int tahun;
    cout << "\n=== Cari Film Berdasarkan Tahun ===\n";
    cout << "Masukkan tahun rilis film yang dicari: ";
    cin >> tahun;
    bool found = false;
    queue<Film> tempQueue = watchlist;
    while (!tempQueue.empty()) {
        Film film = tempQueue.front();
        tempQueue.pop();
        if (film.tahunRilis == tahun) {
            cout << "Film ditemukan: Judul: " << film.judul << ", Tahun Rilis: " << film.tahunRilis << ", Genre: " << film.genre << ", Aktor: ";
            for (int i = 0; i < 3; ++i) {
                cout << *film.aktor[i];
                if (i < 2) cout << ", ";
            }
            cout << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "Film tidak ditemukan.\n";
    }
}

void cariFilmBerdasarkanAktor() {
    string aktor;
    cout << "\n=== Cari Film Berdasarkan Aktor ===\n";
    cout << "Masukkan nama aktor yang dicari: ";
    cin.ignore();
    getline(cin, aktor);
    bool found = false;
    queue<Film> tempQueue = watchlist;
    while (!tempQueue.empty()) {
        Film film = tempQueue.front();
        tempQueue.pop();
        for (int i = 0; i < 3; ++i) {
            if (*film.aktor[i] == aktor) {
                cout << "Film ditemukan: Judul: " << film.judul << ", Tahun Rilis: " << film.tahunRilis << ", Genre: " << film.genre << ", Aktor: ";
                for (int j = 0; j < 3; ++j) {
                    cout << *film.aktor[j];
                    if (j < 2) cout << ", ";
                }
                cout << endl;
                found = true;
                break;
            }
        }
    }
    if (!found) {
        cout << "Film tidak ditemukan.\n";
    }
}

void about(){
    cout << endl<< "=== Anggota Kelompok ===" << endl;
    cout << "1. 23.12.2973 Muhammad Fahmi Aziz Sanjeeda" << endl;
    cout << "2. 23.12.2966 Marcellinus Alfrits Sorongan" << endl;
    cout << "3. 23.12.2925 Panji Ihsanudin Fajri" << endl;
}

int main() {
    string username, password;
    cout << "=== Login ===\n";
    cout << "Masukkan username: ";
    cin >> username;
    cout << "Masukkan password: ";
    cin >> password;

    if (!login(username, password)) {
        cout << "Login gagal. Username atau password salah.\n";
        return 0;
    }

    int pilihan;
    do {
        cout << "\n=== Menu ===\n";
        cout << "1. Tambah Film\n";
        cout << "2. Tampilkan Watchlist\n";
        cout << "3. Cari Film Berdasarkan Judul\n";
        cout << "4. Cari Film Berdasarkan Tahun\n";
        cout << "5. Cari Film Berdasarkan Aktor\n";
        cout << "6. Hapus Film\n";
        cout << "7. Hapus Film Berdasarkan Judul\n";
        cout << "8. Hapus Semua Film\n";
        cout << "9. About Us\n";
        cout << "10. Keluar\n";
		cout << "Pilih opsi: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1:
                tambahFilm();
                break;
            case 2:
                tampilkanWatchlist();
                break;
            case 3:
                cariFilmBerdasarkanJudul();
                break;
            case 4:
                cariFilmBerdasarkanTahun();
                break;
            case 5:
                cariFilmBerdasarkanAktor();
                break;
            case 6:
                hapusFilm();
                break;
            case 7:
                hapusFilmBerdasarkanJudul();
                break;
            case 8:
                hapusSemuaFilm();
                break;
            case 9:
                about();
                break;
            case 10:
                cout << "Keluar dari program.\n";
                break;
            default:
                cout << "Pilihan tidak valid.\n";
        }
    } while (pilihan != 10);

    return 0;
}

