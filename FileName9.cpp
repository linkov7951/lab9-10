#include <iostream>
#include <string>
using namespace std;

enum Comfort { LUX, SEMI_LUX, STANDARD, ECONOMY };

struct Amenities {
    int has_wifi, has_tv, has_bath, has_balcony, has_minibar;
};

struct HotelRoom {
    string hotel_name;
    int room_number;
    Comfort comfort;
    int capacity;
    double price;
    Amenities amenities;
};

string comfort_to_string(Comfort c) {
    if (c == LUX) return "люкс";
    if (c == SEMI_LUX) return "полулюкс";
    if (c == STANDARD) return "стандарт";
    return "эконом";
}

void print_room(HotelRoom r) {
    cout << "\nОтель: " << r.hotel_name << " | Номер: " << r.room_number
        << " | " << comfort_to_string(r.comfort) << " | " << r.capacity << " чел. | "
        << r.price << " руб.\nОпции: Wi-Fi:" << (r.amenities.has_wifi ? "+" : "-")
        << " TV:" << (r.amenities.has_tv ? "+" : "-") << " Ванна:" << (r.amenities.has_bath ? "+" : "-")
        << " Балкон:" << (r.amenities.has_balcony ? "+" : "-") << " Мини-бар:" << (r.amenities.has_minibar ? "+" : "-");
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

void modify_room(HotelRoom& r) {
    cout << "\n--- Изменение номера " << r.room_number << " ---";
    cout << "\nНовое название отеля: "; cin >> r.hotel_name;
    cout << "Новый номер: "; cin >> r.room_number;
    cout << "Комфортность (0-люкс,1-полулюкс,2-стандарт,3-эконом): "; int c; cin >> c; r.comfort = (Comfort)c;
    cout << "Вместимость: "; cin >> r.capacity;
    cout << "Стоимость: "; cin >> r.price;
    cout << "Wi-Fi (1-есть,0-нет): "; cin >> r.amenities.has_wifi;
    cout << "Телевизор: "; cin >> r.amenities.has_tv;
    cout << "Ванна: "; cin >> r.amenities.has_bath;
    cout << "Балкон: "; cin >> r.amenities.has_balcony;
    cout << "Мини-бар: "; cin >> r.amenities.has_minibar;
    cout << "Готово!\n";
}

int filter_by_bath(HotelRoom src[], HotelRoom dst[], int n) {
    int cnt = 0;
    for (int i = 0; i < n; i++) if (src[i].amenities.has_bath) dst[cnt++] = src[i];
    return cnt;
}

void display_filtered(HotelRoom r[], int n, string name) {
    print_rooms(r, n, name);
}

int main() {
    system("chcp 65001 > nul");

    HotelRoom rooms[20] = {
        {"Grand Plaza", 101, LUX, 4, 25000, {1,1,1,1,1}},
        {"Royal House", 102, LUX, 3, 22000, {1,1,1,1,1}},
        {"Golden Palace", 103, LUX, 5, 28000, {1,1,1,1,1}},
        {"Imperial House", 104, LUX, 4, 26000, {1,1,1,1,1}},
        {"Presidential Suite", 105, LUX, 6, 35000, {1,1,1,1,1}},
        {"Sunrise House", 106, SEMI_LUX, 3, 12000, {1,1,1,1,0}},
        {"Sea View", 107, SEMI_LUX, 2, 10000, {1,1,1,0,1}},
        {"Mountain House", 108, SEMI_LUX, 4, 14000, {1,1,1,1,0}},
        {"City Lights", 109, SEMI_LUX, 3, 11000, {1,1,1,0,1}},
        {"Green Valley", 110, SEMI_LUX, 2, 9500, {1,1,1,0,0}},
        {"Central House", 111, STANDARD, 2, 5500, {1,1,0,0,0}},
        {"Metro Inn", 112, STANDARD, 2, 5000, {1,1,0,0,0}},
        {"Park Hotel", 113, STANDARD, 3, 6000, {1,1,1,0,0}},
        {"Business Class", 114, STANDARD, 2, 5800, {1,1,0,0,0}},
        {"Comfort Inn", 115, STANDARD, 3, 6500, {1,1,1,0,0}},
        {"Budget House", 116, ECONOMY, 1, 2000, {0,1,0,0,0}},
        {"Simple Stay", 117, ECONOMY, 1, 1800, {0,1,0,0,0}},
        {"Hostel Style", 118, ECONOMY, 2, 2500, {1,0,0,0,0}},
        {"Economy Inn", 119, ECONOMY, 1, 1900, {0,1,0,0,0}},
        {"Traveller House", 120, ECONOMY, 2, 2700, {1,0,0,0,0}}
    };

    print_rooms(rooms, 20, "ВСЕ НОМЕРА");

    HotelRoom house_rooms[20];
    int hc = filter_by_hotel_name(rooms, house_rooms, 20);
    print_rooms(house_rooms, hc, "ОТЕЛИ НА 'house'");

    sort_by_capacity(house_rooms, hc);
    print_rooms(house_rooms, hc, "ПОСЛЕ СОРТИРОВКИ ПО ВМЕСТИМОСТИ");

    cout << "\n=== ПОИСК НОМЕРА 107 ===";
    print_room_by_number(rooms, 20, 107);

    print_top3_expensive_lux(rooms, 20);

    HotelRoom bath_rooms[20];
    int bc = filter_by_bath(rooms, bath_rooms, 20);
    display_filtered(bath_rooms, bc, "НОМЕРА С ВАННОЙ");

    return 0;
}