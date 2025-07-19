#include <iostream>
#include <cstring>
#include <limits>

struct Character {
    char name[20];
    int level;
    int hp;
    int atk;
    int def;
    int charClass;
};

int strLen(const char* str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

void strCopy(char* dest, const char* src) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
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
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            int k = 0;
            while (chars[j].name[k] && chars[j+1].name[k] && 
                   chars[j].name[k] == chars[j+1].name[k]) {
                k++;
            }
            if (chars[j].name[k] > chars[j+1].name[k]) {
                Character temp = chars[j];
                chars[j] = chars[j + 1];
                chars[j + 1] = temp;
            }
        }
    }
}

Character* findCharacter(Character* chars, int size, const char* name) {
    for (int i = 0; i < size; i++) {
        if (strCompare(chars[i].name, name)) {
            return &chars[i];
        }
    }
    return nullptr;
}

void printCharacter(const Character& character) {
    std::cout << character.name << "\t" << character.level << "\t" 
         << character.hp << "\t" << character.atk << "\t" << character.def << "\t";
        switch (character.charClass) {
            case 0: std::cout << "None"; break;
            case 1: std::cout << "Warrior"; break;
            case 2: std::cout << "Mage"; break;
            case 3: std::cout << "Rogue"; break;
            default: break;
    }
    std::cout << std::endl;
}

void printAllCharacters(Character* chars, int size) {
    if (size == 0) {
        std::cout << "No characters available." << std::endl;
        return;
    }
    
    std::cout << "Name\tLVL\tHP\tATK\tDEF\tClass" << std::endl;
    sortCharacters(chars, size);
    for (int i = 0; i < size; i++) {
        printCharacter(chars[i]);
    }
}

bool addCharacter(Character* chars, int& size) {
    if (size >= 10) {
        std::cout << "Maximum number of characters reached." << std::endl;
        return false;
    }

    Character newChar;
    newChar.charClass = 0;

    std::cout << "Enter name: ";
    std::cin.getline(newChar.name, 20);
    
    std::cout << "Enter level (1-100): ";
    while (!(std::cin >> newChar.level) || newChar.level < 1 || newChar.level > 100) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Enter level (1-100): ";
    }
    std::cin.ignore();

    std::cout << "Choose class (1-Warrior, 2-Mage, 3-Rogue, 0-None): ";
    while (!(std::cin >> newChar.charClass) || newChar.charClass < 0 || newChar.charClass > 3) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Choose class (1-Warrior, 2-Mage, 3-Rogue, 0-None): ";
    }
    std::cin.ignore();

    newChar.atk = 15;
    newChar.def = 10;

    switch (newChar.charClass) {
        case 1: newChar.def += 5; break;
        case 2: newChar.atk += 5; break;
        case 3: newChar.atk += 3; newChar.def += 2; break;
        default: break;
    }

    newChar.hp = 100 + (newChar.level * (newChar.def / 10));
    chars[size++] = newChar;
    return true;
}

void modifyCharacter(Character* chars, int size) {
    char name[20];
    std::cout << "Enter character name: ";
    std::cin.getline(name, 20);

    Character* character = findCharacter(chars, size, name);
    if (character == nullptr) {
        std::cout << "Character not found." << std::endl;
        return;
    }

    std::cout << "Choose parameter to modify:" << std::endl;
    std::cout << "1. Increase level" << std::endl;
    std::cout << "2. Change ATK" << std::endl;
    std::cout << "3. Change DEF" << std::endl;
    std::cout << "> ";

    int choice;
    std::cin >> choice;
    std::cin.ignore();

    switch (choice) {
        case 1: {
            std::cout << "Enter new level: ";
            int newLevel;
            while (!(std::cin >> newLevel) || newLevel < 1 || newLevel > 100) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Enter level (1-100): ";
            }
            character->level = newLevel;
            character->hp = 100 + (character->level * (character->def / 10));
            std::cout << "Level updated. New HP: " << character->hp << std::endl;
            break;
        }
        case 2: {
            std::cout << "Enter new ATK (10-50): ";
            int newAtk;
            while (!(std::cin >> newAtk) || newAtk < 10 || newAtk > 50) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Enter ATK (10-50): ";
            }
            character->atk = newAtk;
            std::cout << "ATK updated." << std::endl;
            break;
        }
        case 3: {
            std::cout << "Enter new DEF (1-50): ";
            int newDef;
            while (!(std::cin >> newDef) || newDef < 1 || newDef > 50) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Enter DEF (1-50): ";
            }
            character->def = newDef;
            character->hp = 100 + (character->level * (character->def / 10));
            std::cout << "DEF updated. New HP: " << character->hp << std::endl;
            break;
        }
        default:
            std::cout << "Invalid choice." << std::endl;
    }
}

void findCharactersByStats(const Character* chars, int size, int min_atk, int min_def) {
    bool found = false;
    std::cout << "Characters with ATK >= " << min_atk << " and DEF >= " << min_def << ":" << std::endl;
    std::cout << "Name\tLVL\tHP\tATK\tDEF\tClass" << std::endl;
    
    for (int i = 0; i < size; i++) {
        if (chars[i].atk >= min_atk && chars[i].def >= min_def) {
            printCharacter(chars[i]);
            found = true;
        }
    }
    
    if (!found) {
        std::cout << "No characters found with these stats." << std::endl;
    }
}

void battleCharacters(Character* chars, int size) {
    char name1[20], name2[20];
    
    std::cout << "Enter first character name: ";
    std::cin.getline(name1, 20);
    Character* char1 = findCharacter(chars, size, name1);
    if (char1 == nullptr) {
        std::cout << "First character not found." << std::endl;
        return;
    }
    
    std::cout << "Enter second character name: ";
    std::cin.getline(name2, 20);
    Character* char2 = findCharacter(chars, size, name2);
    if (char2 == nullptr) {
        std::cout << "Second character not found." << std::endl;
        return;
    }
    
    int power1 = char1->atk - char2->def;
    int power2 = char2->atk - char1->def;
    
    std::cout << "Battle results:" << std::endl;
    
    if (power1 > power2) {
        std::cout << char1->name << " win" << std::endl;
    } else if (power2 > power1) {
        std::cout << char2->name << " win" << std::endl;
    } else {
        std::cout << "draw" << std::endl;
    }
}

int main() {
    Character characters[10];
    int characterCount = 0;
    
    while (true) {
        std::cout << "1. Add character" << std::endl;
        std::cout << "2. Find character" << std::endl;
        std::cout << "3. Show all characters" << std::endl;
        std::cout << "4. Modify character" << std::endl;
        std::cout << "5. Find by stats" << std::endl;
        std::cout << "6. Battle characters" << std::endl;
        std::cout << "0. Exit" << std::endl;
        std::cout << "> ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        switch (choice) {
            case 1:
                addCharacter(characters, characterCount);
                break;
            case 2: {
                char name[20];
                std::cout << "Enter character name: ";
                std::cin.getline(name, 20);
                Character* found = findCharacter(characters, characterCount, name);
                if (found != nullptr) {
                    std::cout << "Character found:" << std::endl;
                    std::cout << "Name\t\tLVL\tHP\tATK\tDEF\tClass" << std::endl;
                    printCharacter(*found);
                } else {
                    std::cout << "Character not found." << std::endl;
                }
                break;
            }
            case 3:
                printAllCharacters(characters, characterCount);
                break;
            case 4:
                modifyCharacter(characters, characterCount);
                break;
            case 5: {
                int min_atk, min_def;
                std::cout << "Enter minimum ATK: ";
                std::cin >> min_atk;
                std::cout << "Enter minimum DEF: ";
                std::cin >> min_def;
                std::cin.ignore();
                findCharactersByStats(characters, characterCount, min_atk, min_def);
                break;
            }
            case 6:
                battleCharacters(characters, characterCount);
                break;
            case 0:
                return 0;
            default:
                std::cout << "Invalid choice." << std::endl;
        }
    }
    
    return 0;
}