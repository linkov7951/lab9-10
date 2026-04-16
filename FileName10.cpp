#include <iostream>
#include <fstream>
#include <string>
using namespace std;

enum Comfort { LUX, SEMI_LUX, STANDARD, ECONOMY };

struct Amenities {
    int has_wifi;
    int has_tv;
    int has_bath;
    int has_balcony;
    int has_minibar;

    Amenities() : has_wifi(0), has_tv(0), has_bath(0), has_balcony(0), has_minibar(0) {}

    Amenities(int wifi, int tv, int bath, int balcony, int minibar)
        : has_wifi(wifi), has_tv(tv), has_bath(bath), has_balcony(balcony), has_minibar(minibar) {}
};

struct HotelRoom {
    string hotel_name;
    int room_number;
    Comfort comfort;
    int capacity;
    double price;
    Amenities amenities;

    HotelRoom()
        : hotel_name(""),
        room_number(0),
        comfort(ECONOMY),
        capacity(0),
        price(0.0),
        amenities() {}

    HotelRoom(string name, int num, Comfort comp, int cap, double pr, const Amenities& am)
        : hotel_name(name),
        room_number(num),
        comfort(comp),
        capacity(cap),
        price(pr),
        amenities(am) {}
};

string comfort_to_string(Comfort c) {
    if (c == LUX) return "люкс";
    if (c == SEMI_LUX) return "полулюкс";
    if (c == STANDARD) return "стандарт";
    return "эконом";
}

void print_room(const HotelRoom& r) {
    cout << "\nОтель: " << r.hotel_name << " | Номер: " << r.room_number
        << " | " << comfort_to_string(r.comfort) << " | " << r.capacity << " чел. | "
        << r.price << " руб.\nОпции: Wi-Fi:" << (r.amenities.has_wifi ? "+" : "-")
        << " TV:" << (r.amenities.has_tv ? "+" : "-") << " Ванна:" << (r.amenities.has_bath ? "+" : "-")
        << " Балкон:" << (r.amenities.has_balcony ? "+" : "-") << " Мини-бар:" << (r.amenities.has_minibar ? "+" : "-");
}

void update_prices_from_file(HotelRoom rooms[], int n, const string& filename) {
    ifstream fin(filename);
    if (!fin.is_open()) {
        cout << "Ошибка открытия файла " << filename << endl;
        return;
    }

    int room_num;
    double new_price;
    while (fin >> room_num >> new_price) {
        for (int i = 0; i < n; i++) {
            if (rooms[i].room_number == room_num) {
                rooms[i].price = new_price;
                cout << "Обновлена цена номера " << room_num << " -> " << new_price << " руб.\n";
                break;
            }
        }
    }
    fin.close();
}

void save_to_binary(const HotelRoom& room, const string& filename) {
    ofstream out(filename, ios::binary | ios::app);
    if (!out.is_open()) {
        cout << "Ошибка открытия бинарного файла для записи\n";
        return;
    }

    size_t len = room.hotel_name.length();
    out.write((char*)&len, sizeof(len));
    out.write(room.hotel_name.c_str(), len);
    out.write((char*)&room.room_number, sizeof(room.room_number));
    out.write((char*)&room.comfort, sizeof(room.comfort));
    out.write((char*)&room.capacity, sizeof(room.capacity));
    out.write((char*)&room.price, sizeof(room.price));
    out.write((char*)&room.amenities, sizeof(room.amenities));

    out.close();
    cout << "Номер " << room.room_number << " сохранён в бинарный файл\n";
}

bool read_from_binary(HotelRoom& room, const string& filename, int pos = 0) {
    ifstream in(filename, ios::binary);
    if (!in.is_open()) return false;

    for (int i = 0; i < pos; i++) {
        size_t len;
        in.read((char*)&len, sizeof(len));
        in.seekg(len, ios::cur);
        in.seekg(sizeof(room.room_number) + sizeof(room.comfort) +
            sizeof(room.capacity) + sizeof(room.price) + sizeof(room.amenities), ios::cur);
        if (in.eof()) return false;
    }

    size_t len;
    in.read((char*)&len, sizeof(len));
    char* buf = new char[len + 1];
    in.read(buf, len);
    buf[len] = '\0';
    room.hotel_name = buf;
    delete[] buf;

    in.read((char*)&room.room_number, sizeof(room.room_number));
    in.read((char*)&room.comfort, sizeof(room.comfort));
    in.read((char*)&room.capacity, sizeof(room.capacity));
    in.read((char*)&room.price, sizeof(room.price));
    in.read((char*)&room.amenities, sizeof(room.amenities));

    in.close();
    return true;
}

void print_rooms(HotelRoom r[], int n, string title) {
    cout << "\n=== " << title << " (" << n << " шт) ===";
    for (int i = 0; i < n; i++) print_room(r[i]);
    cout << "\n";
}

int filter_by_hotel_name(HotelRoom src[], HotelRoom dst[], int n) {
    int cnt = 0;
    for (int i = 0; i < n; i++) {
        string name = src[i].hotel_name;
        if (name.length() >= 6 && name.substr(name.length() - 6) == " House") {
            dst[cnt++] = src[i];
        }
    }
    return cnt;
}

void sort_by_capacity(HotelRoom r[], int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (r[j].capacity > r[j + 1].capacity) {
                HotelRoom t = r[j];
                r[j] = r[j + 1];
                r[j + 1] = t;
            }
}

void print_room_by_number(HotelRoom r[], int n, int num) {
    for (int i = 0; i < n; i++)
        if (r[i].room_number == num) {
            print_room(r[i]);
            return;
        }
    cout << "\nНомер " << num << " не найден";
}

void print_top3_expensive_lux(HotelRoom r[], int n) {
    HotelRoom lux[20];
    int cnt = 0;
    for (int i = 0; i < n; i++) if (r[i].comfort == LUX) lux[cnt++] = r[i];

    for (int i = 0; i < cnt - 1; i++)
        for (int j = 0; j < cnt - i - 1; j++)
            if (lux[j].price < lux[j + 1].price) {
                HotelRoom t = lux[j];
                lux[j] = lux[j + 1];
                lux[j + 1] = t;
            }

    cout << "\n=== 3 самых дорогих люкса ===";
    for (int i = 0; i < 3 && i < cnt; i++) {
        cout << "\n" << i + 1 << ". " << lux[i].hotel_name << " (№" << lux[i].room_number
            << ") - " << lux[i].price << " руб.\n   Опции: ванна:" << (lux[i].amenities.has_bath ? "+" : "-")
            << " балкон:" << (lux[i].amenities.has_balcony ? "+" : "-") << " мини-бар:" << (lux[i].amenities.has_minibar ? "+" : "-");
    }
}

int filter_by_bath(HotelRoom src[], HotelRoom dst[], int n) {
    int cnt = 0;
    for (int i = 0; i < n; i++) if (src[i].amenities.has_bath) dst[cnt++] = src[i];
    return cnt;
}

int main() {
    system("chcp 65001 > nul");

    HotelRoom rooms[20] = {
        HotelRoom("Grand Plaza", 101, LUX, 4, 25000, Amenities(1,1,1,1,1)),
        HotelRoom("Royal House", 102, LUX, 3, 22000, Amenities(1,1,1,1,1)),
        HotelRoom("Golden Palace", 103, LUX, 5, 28000, Amenities(1,1,1,1,1)),
        HotelRoom("Imperial House", 104, LUX, 4, 26000, Amenities(1,1,1,1,1)),
        HotelRoom("Presidential Suite", 105, LUX, 6, 35000, Amenities(1,1,1,1,1)),
        HotelRoom("Sunrise House", 106, SEMI_LUX, 3, 12000, Amenities(1,1,1,1,0)),
        HotelRoom("Sea View", 107, SEMI_LUX, 2, 10000, Amenities(1,1,1,0,1)),
        HotelRoom("Mountain House", 108, SEMI_LUX, 4, 14000, Amenities(1,1,1,1,0)),
        HotelRoom("City Lights", 109, SEMI_LUX, 3, 11000, Amenities(1,1,1,0,1)),
        HotelRoom("Green Valley", 110, SEMI_LUX, 2, 9500, Amenities(1,1,1,0,0)),
        HotelRoom("Central House", 111, STANDARD, 2, 5500, Amenities(1,1,0,0,0)),
        HotelRoom("Metro Inn", 112, STANDARD, 2, 5000, Amenities(1,1,0,0,0)),
        HotelRoom("Park Hotel", 113, STANDARD, 3, 6000, Amenities(1,1,1,0,0)),
        HotelRoom("Business Class", 114, STANDARD, 2, 5800, Amenities(1,1,0,0,0)),
        HotelRoom("Comfort Inn", 115, STANDARD, 3, 6500, Amenities(1,1,1,0,0)),
        HotelRoom("Budget House", 116, ECONOMY, 1, 2000, Amenities(0,1,0,0,0)),
        HotelRoom("Simple Stay", 117, ECONOMY, 1, 1800, Amenities(0,1,0,0,0)),
        HotelRoom("Hostel Style", 118, ECONOMY, 2, 2500, Amenities(1,0,0,0,0)),
        HotelRoom("Economy Inn", 119, ECONOMY, 1, 1900, Amenities(0,1,0,0,0)),
        HotelRoom("Traveller House", 120, ECONOMY, 2, 2700, Amenities(1,0,0,0,0))
    };

    ofstream("price_update.txt") << "101 27000\n105 38000\n112 5200\n";
    cout << "\n=== ЗАДАНИЕ 1: обновление цен из файла ===\n";
    update_prices_from_file(rooms, 20, "price_update.txt");

    cout << "\n=== ЗАДАНИЕ 2: запись в бинарный файл ===\n";
    save_to_binary(rooms[0], "hotel_rooms.bin");
    save_to_binary(rooms[5], "hotel_rooms.bin");

    cout << "\n=== Чтение из бинарного файла ===\n";
    HotelRoom loaded_room;
    if (read_from_binary(loaded_room, "hotel_rooms.bin", 0)) {
        cout << "Первая запись из бинарного файла:";
        print_room(loaded_room);
    }
    if (read_from_binary(loaded_room, "hotel_rooms.bin", 1)) {
        cout << "\nВторая запись из бинарного файла:";
        print_room(loaded_room);
    }

    print_rooms(rooms, 20, "ВСЕ НОМЕРА (после обновления цен)");

    HotelRoom house_rooms[20];
    int hc = filter_by_hotel_name(rooms, house_rooms, 20);
    sort_by_capacity(house_rooms, hc);
    print_rooms(house_rooms, hc, "ОТЕЛИ НА 'House' (сортировка по вместимости)");

    print_room_by_number(rooms, 20, 107);
    print_top3_expensive_lux(rooms, 20);

    HotelRoom bath_rooms[20];
    int bc = filter_by_bath(rooms, bath_rooms, 20);
    print_rooms(bath_rooms, bc, "НОМЕРА С ВАННОЙ");

    cout << "\n=== ГОТОВО ===" << endl;
    return 0;
}