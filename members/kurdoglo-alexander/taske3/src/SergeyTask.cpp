#include <iostream>
#include <cstring>
#include <limits>
#include <random>

struct Character
{
    char name[20];
    int lvl;
    int hp;
    int atk;
    int def;
    int characterClass;
};

int myStrlen(char* str)
{
    int strLen = 0;
    while (str[strLen] != 0) {
        strLen++;
    }
    return strLen;
}

Character* isCharacterExist(Character* characters, char* characterName, int numberOfCharacters)
{
    if (characterName == nullptr || numberOfCharacters < 1) {
        return nullptr;
    }

    for (int i = 0; i < numberOfCharacters; i++) {
        if (strcmp(characters[i].name, characterName) == 0) {
            return &characters[i];
        }
    }

    return nullptr;
}

void sortArrayByASCII(Character* characters, int numberOfCharacters)
{
    for (int i = 0; i < numberOfCharacters - 1; i++) {
        for (int j = 0; j < numberOfCharacters - i - 1; j++) {
            if (strcmp(characters[j].name, characters[j + 1].name) > 0) {
                Character temp = characters[j];
                characters[j] = characters[j + 1];
                characters[j + 1] = temp;
            }
        }
    }
}

void printAllCharacters(Character* characters, int numberOfCharacters)
{
    if (numberOfCharacters == 0) {
        std::cout << "No character in characters list!\n";
        return;
    }

    sortArrayByASCII(characters, numberOfCharacters);
    std::cout << "Name\tLVL\tHP\tATK\tDEF\n";
    for (int i = 0; i < numberOfCharacters; i++) {
        std::cout << characters[i].name << "\t";
        std::cout << characters[i].lvl << "\t"
            << characters[i].hp << "\t"
            << characters[i].atk << "\t"
            << characters[i].def << "\n";
    }
}

bool addCharacter(Character* characters, int& numberOfCharacters)
{
    if (numberOfCharacters >= 10) {
        std::cout << "Character stack overflow!\n";
        return false;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(10, 50);

    Character newCharacter;

    // Ввод имени
    do {
        std::cout << "Enter character name:\n";
        std::cin.getline(newCharacter.name, 20);
        if (myStrlen(newCharacter.name) == 0) {
            std::cout << "Character name cannot be empty!\n";
            continue;
        }
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Character name is too long. The maximum length of a name is 19 characters\n";
            continue;
        }
        if (isCharacterExist(characters, newCharacter.name, numberOfCharacters) != nullptr) {
            std::cout << "Character with this name already exists! Please rename character\n";
            continue;
        }
        break;

    } while (true);

    // Ввод уровня
    do {
        std::cout << "Enter character level (minimum 1, maximum 100)\n";
        if (!(std::cin >> newCharacter.lvl)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Input number in range 1 - 100\n";
            continue;
        }
        if (newCharacter.lvl < 1 || newCharacter.lvl > 100) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Character level out of range\n";
            continue;
        }
        break;
    } while (true);

    // Ввод класса персонажа
    do {
        std::cout << "Enter character class:\n0 - Classless\n1 - Warrior\n2 - Mage\n3 - Rogue\n";
        if (!(std::cin >> newCharacter.characterClass)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Input number in range 0 - 3\n";
            continue;
        }
        if (newCharacter.characterClass < 0 || newCharacter.characterClass > 3) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Character class out of range\n";
            continue;
        }
        break;
    } while (true);

    newCharacter.def = dist(gen);
    newCharacter.atk = dist(gen);
    newCharacter.hp = 100 + (newCharacter.lvl * newCharacter.def) / 10;

    switch (newCharacter.characterClass) {
    case 1:
        newCharacter.def += 5;
        break;
    case 2:
        newCharacter.atk += 5;
        break;
    case 3:
        newCharacter.def += 2;
        newCharacter.atk += 3;
        break;
    }

    characters[numberOfCharacters] = newCharacter;
    numberOfCharacters++;

    std::cout << "The character was successfully created!\n";
    return true;
}

void changeCharacterCharacteristics(Character* characters, int numberOfCharacters)
{
    char characterName[20];
    std::cout << "Enter character name: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.getline(characterName, 20);
    if (myStrlen(characterName) == 0) {
        std::cout << "Name cannot be empty\n";
        return;
    }

    Character* modifiedCharacter = isCharacterExist(characters, characterName, numberOfCharacters);
    if (modifiedCharacter == nullptr) {
        std::cout << "Character with this name does not exist\n";
        return;
    }

    int userChoose;
    std::cout << "Choose characteristics to change\n0 - level\n1 - attack\n2 - defence\n";
    std::cin >> userChoose;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    switch (userChoose) {
    case 0:
        std::cout << "Enter character level (minimum 1, maximum 100)\n";
        std::cin >> modifiedCharacter->lvl;
        if (modifiedCharacter->lvl < 1 || modifiedCharacter->lvl > 100) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Character level out of range\n";
            return;
        }
        modifiedCharacter->hp = 100 + (modifiedCharacter->lvl * modifiedCharacter->def) / 10;
        std::cout << "New character level: " << modifiedCharacter->lvl << "\n";
        std::cout << "New character hp: " << modifiedCharacter->hp << "\n";
        return;
    case 1:
        std::cout << "Enter character attack\n";
        std::cin >> modifiedCharacter->atk;
        if (modifiedCharacter->atk < 1 || modifiedCharacter->atk > 50) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Character attack out of range\n";
            return;
        }
        std::cout << "New character attack: " << modifiedCharacter->atk << "\n";
        return;
    case 2:
        std::cout << "Enter character defence\n";
        std::cin >> modifiedCharacter->def;
        if (modifiedCharacter->def < 1 || modifiedCharacter->def > 50) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Character defence out of range\n";
            return;
        }
        modifiedCharacter->hp = 100 + (modifiedCharacter->lvl * modifiedCharacter->def) / 10;
        std::cout << "New character defence: " << modifiedCharacter->def << "\n";
        std::cout << "New character hp: " << modifiedCharacter->hp << "\n";
        return;
    default:
        std::cout << "Something went wrong...\n";
        return;
    }
}

void findCharactersByStats(Character* characters, int size, int minAtk, int minDef) {
    bool anyCharacterExist = false;
    std::cout << "Name\tLVL\tHP\tATK\tDEF\n";
    for (int i = 0; i < size; i++) {
        if (characters[i].atk >= minAtk && characters[i].def >= minDef) {
            std::cout << characters[i].name << "\t";
            std::cout << characters[i].lvl << "\t"
                << characters[i].hp << "\t"
                << characters[i].atk << "\t"
                << characters[i].def << "\n";
            anyCharacterExist = true;
        }
    }
    if (!anyCharacterExist) {
        std::cout << "None found\n";
    }
}

void battleCharacters(Character* characters, int numberOfCharacters) {
    char name1[20], name2[20];
    int fightPoint;

    std::cout << "Enter first character name: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.getline(name1, 20);
    if (myStrlen(name1) == 0) {
        std::cout << "Name cannot be empty\n";
        return;
    }
    Character* character1 = isCharacterExist(characters, name1, numberOfCharacters);
    if (character1 == nullptr) {
        std::cout << "Character does not exist\n";
        return;
    }

    std::cout << "Enter second character name: ";
    std::cin.getline(name2, 20);
    if (myStrlen(name2) == 0) {
        std::cout << "Name cannot be empty\n";
        return;
    }
    Character* character2 = isCharacterExist(characters, name2, numberOfCharacters);
    if (character2 == nullptr) {
        std::cout << "Character does not exist\n";
        return;
    }

    if (character1 == character2) {
        std::cout << "Что за новая форма самобичевания лол";
        return;
    }

    int power1 = character1->atk - character2->def;
    int power2 = character2->atk - character1->def;

    if (power1 > power2) {
        std::cout << character1->name << " wins\n";
    }
    else if (power2 > power1) {
        std::cout << character2->name << " wins\n";
    }
    else {
        std::cout << "Draw\n";
    }

}

int main()
{
    Character characters[10];
    int numberOfCharacters = 0;

    while (true) {
        std::cout << "1. Add character\n"
            << "2. Find character\n"
            << "3. Show all characters\n"
            << "4. Modify character\n"
            << "5. Find characters by stats\n"
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
        case 1:
            addCharacter(characters, numberOfCharacters);
            break;
        case 2: {
            char name[20];
            std::cout << "Enter character name: ";
            std::cin.getline(name, 20);
            if (myStrlen(name) == 0) {
                std::cout << "Name cannot be empty\n";
                break;
            }
            Character* characterExist = isCharacterExist(characters, name, numberOfCharacters);
            if (characterExist != nullptr) {
                std::cout << "Character exists\n";
                break;
            }
            std::cout << "Character does not exist\n";
            break;
        }
        case 3:
            printAllCharacters(characters, numberOfCharacters);
            break;
        case 4:
            changeCharacterCharacteristics(characters, numberOfCharacters);
            break;
        case 5: {
            int min_atk, min_def;
            std::cout << "Enter minimum ATK: ";
            std::cin >> min_atk;
            std::cout << "Enter minimum DEF: ";
            std::cin >> min_def;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            findCharactersByStats(characters, numberOfCharacters, min_atk, min_def);
            break;
        }
        case 6:
            battleCharacters(characters, numberOfCharacters);
            break;
        case 0:
            return 0;
        default:
            std::cout << "Invalid choice\n";
        }
    }

    return 0;
}
