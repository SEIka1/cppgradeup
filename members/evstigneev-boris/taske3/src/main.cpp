#include <iostream>
#include <cstring>
#include <limits>

struct Character {
    char name[20];
    int level;
    int hp;
    int atk;
    int def;
    int charClass; // 0 - None, 1 - Warrior, 2 - Mage, 3 - Rogue
};

int strLen(const char* str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

void strCpy_s(char* dest, size_t destSize, const char* src) {
    if (!dest || !src || destSize == 0) return;

    size_t i = 0;
    while (src[i] != '\0') {
        if (i >= destSize - 1) {
            dest[0] = '\0';
            return;
        }
        dest[i] = src[i];
        ++i;
    }
    dest[i] = '\0';
    return;
}

bool strCompare(const char* str1, const char* str2) {
    while (*str1 && *str2) {
        if (*str1 != *str2) {
            return false;
        }
        str1++;
        str2++;
    }
    return (*str1 == '\0' && *str2 == '\0');
}

void sortCharacters(Character* chars, int size) {
    if (chars == nullptr || size <= 1) return;

    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            int k = 0;
            while (k < 20 && chars[j].name[k] != '\0' && chars[j+1].name[k] != '\0') {
                char c1 = tolower(chars[j].name[k]);
                char c2 = tolower(chars[j+1].name[k]);
                
                if (c1 != c2) {
                    if (c1 > c2) {
                        Character temp = chars[j];
                        chars[j] = chars[j+1];
                        chars[j+1] = temp;
                    }
                    break; 
                }
                k++;
            }
        }
    }
}

Character* findCharacter(Character* chars, int size, const char* name) {
    if (chars == nullptr || size <= 0 || name == nullptr) return nullptr;

    for (int i = 0; i < size; ++i) {
        if (strCompare(chars[i].name, name)) {
            return &chars[i];
        }
    }
    return nullptr;
}
void printCharacter(const Character& character) {
    if (character.name[0] == '\0') {
        std::cout << "(no name)\t";
    } else {
        std::cout << character.name << "\t";
    }

    std::cout << character.level << "\t" 
              << character.hp << "\t" 
              << character.atk << "\t" 
              << character.def << "\t";
              
    switch (character.charClass) {
        case 0: std::cout << "None"; break;
        case 1: std::cout << "Warrior"; break;
        case 2: std::cout << "Mage"; break;
        case 3: std::cout << "Rogue"; break;
        default: std::cout << "Unknown"; break;
    }
    std::cout << "\n";
}

void printAllCharacters(Character* chars, int size) {
    if (size == 0) {
        std::cout << "No characters available\n";
        return;
    }
    sortCharacters(chars, size);
    std::cout << "Name\tLVL\tHP\tATK\tDEF\tClass\n";
    for (int i = 0; i < size; ++i) printCharacter(chars[i]);
}

bool readIntInRange(int& value, int min, int max) {
    if (!(std::cin >> value) || value < min || value > max) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    return true;
}

bool addCharacter(Character* chars, int& size) {
    if (size >= 10) {
        std::cout << "Maximum number of characters reached\n";
        return false;
    }

    Character newChar;
    newChar.charClass = 0;

    std::cout << "Enter name: ";
    while (true) {
        std::cin.getline(newChar.name, 20);
        if (strLen(newChar.name) == 0) {
            std::cout << "Name cannot be empty. Enter name: ";
            continue;
        }
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Too long. Enter name: ";
            continue;
        }
        if (findCharacter(chars, size, newChar.name) != nullptr) {
            std::cout << "Character with this name already exists. Enter name: ";
            continue;
        }
        break;
    }
    
    std::cout << "Enter level (1-100): ";
    while (!readIntInRange(newChar.level, 1, 100)) {
        std::cout << "Invalid input. Enter level (1-100): ";
    }

    std::cout << "Choose class (1-Warrior, 2-Mage, 3-Rogue, 0-None): ";
    while (!readIntInRange(newChar.charClass, 0, 3)) {
        std::cout << "Invalid input. Choose class (0-3): ";
    }

    newChar.atk = 15;
    newChar.def = 10;
    
    switch (newChar.charClass) {
        case 1: newChar.def += 5; break;
        case 2: newChar.atk += 5; break;
        case 3: newChar.atk += 3; newChar.def += 2; break;
        default: break;
    }
    
    newChar.hp = 100 + (newChar.level * newChar.def) / 10;
    
    strCpy_s(chars[size].name, sizeof(chars[size].name), newChar.name);
    chars[size].level = newChar.level;
    chars[size].hp = newChar.hp;
    chars[size].atk = newChar.atk;
    chars[size].def = newChar.def;
    chars[size].charClass = newChar.charClass;
    ++size;
    
    return true;
}

void modifyCharacter(Character* chars, int size) {
    char name[20];
    std::cout << "Enter character name: ";
    std::cin.getline(name, 20);

    Character* newChar = findCharacter(chars, size, name);
    if (!newChar) {
        std::cout << "Character not found\n";
        return;
    }

    std::cout << "Modify:\n1.Level\n2.ATK\n3.DEF\n> ";
    int choice;
    if (!readIntInRange(choice, 1, 3)) {
        std::cout << "Invalid choice\n";
        return;
    }

    switch (choice) {
        case 1:
            std::cout << "New level (1-100): ";
            while (!readIntInRange(newChar->level, 1, 100)) {
                std::cout << "Invalid input. Enter new level (1-100): ";
            }
            newChar->hp = 100 + (newChar->level * newChar->def) / 10;
            break;
        case 2:
            std::cout << "New ATK (10-50): ";
            while (!readIntInRange(newChar->atk, 10, 50)) {
                std::cout << "Invalid input. Enter new ATK (1-50): ";
            }
            break;
        case 3:
            std::cout << "New DEF (1-50): ";
            while (!readIntInRange(newChar->def, 1, 50)) {
                std::cout << "Invalid input. Enter new DEF (1-50): ";
            }
            newChar->hp = 100 + (newChar->level * newChar->def) / 10;
            break;
    }
}


void findCharactersByStats(const Character* chars, int size, int minAtk, int minDef) {
    bool found = false;
    std::cout << "Matching characters:\nName\tLVL\tHP\tATK\tDEF\tClass\n";
    for (int i = 0; i < size; ++i) {
        if (chars[i].atk >= minAtk && chars[i].def >= minDef) {
            printCharacter(chars[i]);
            found = true;
        }
    }
    if (!found) std::cout << "None found\n";
}

void battleCharacters(Character* chars, int size) {
    char name1[20], name2[20];

    std::cout << "Enter first character name: ";
    std::cin.getline(name1, 20);
    Character* char1 = findCharacter(chars, size, name1);
    if (char1 == nullptr) {
        std::cout << "First character not found\n";
        return;
    }

    std::cout << "Enter second character name: ";
    std::cin.getline(name2, 20);
    Character* char2 = findCharacter(chars, size, name2);
    if (char2 == nullptr) {
        std::cout << "Second character not found\n";
        return;
    }

    int power1 = char1->atk - char2->def;
    int power2 = char2->atk - char1->def;

    std::cout << "Battle results:\n";

    if (power1 > power2) {
        std::cout << char1->name << " win\n";
    } else if (power2 > power1) {
        std::cout << char2->name << " win\n";
    } else {
        std::cout << "draw\n";
    }
}

void handleFindCharacter(Character* chars, int count) {
    char name[20];
    std::cout << "Enter character name: ";
    std::cin.getline(name, 20);
    Character* found = findCharacter(chars, count, name);
    if (found != nullptr) {
        std::cout << "Character found:\n";
        std::cout << "Name\t\tLVL\tHP\tATK\tDEF\tClass\n";
        printCharacter(*found);
    } else {
        std::cout << "Character not found\n";
    }
}

void handleFindByStats(Character* chars, int count) {
    int min_atk, min_def;
    std::cout << "Enter minimum ATK: ";
    std::cin >> min_atk;
    std::cout << "Enter minimum DEF: ";
    std::cin >> min_def;
    std::cin.ignore();
    findCharactersByStats(chars, count, min_atk, min_def);
}

int main() {
    Character characters[10];
    int characterCount = 0;

    while (true) {
        std::cout << "1. Add character\n"
                  << "2. Find character\n"
                  << "3. Show all characters\n"
                  << "4. Modify character\n"
                  << "5. Find by stats\n"
                  << "6. Battle characters\n"
                  << "0. Exit\n"
                  << "> ";
        
        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1: addCharacter(characters, characterCount); break;
            case 2: handleFindCharacter(characters, characterCount); break;
            case 3: printAllCharacters(characters, characterCount); break;
            case 4: modifyCharacter(characters, characterCount); break;
            case 5: handleFindByStats(characters, characterCount); break;
            case 6: battleCharacters(characters, characterCount); break;
            case 0: return 0;
            default: std::cout << "Invalid choice\n";
        }
    }

    return 0;
}