#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <iomanip>

using namespace std;

class Character {
private:
    int health;
    int attack;
    int defense;

public:
    Character(int health, int attack, int defense) : health(health), attack(attack), defense(defense) {}

    void displayStats() const {
        cout << "Character Stats:" << endl;
        cout << "Health: " << health << endl;
        cout << "Attack: " << attack << endl;
        cout << "Defense: " << defense << endl;
    }

    void modifyStats(int healthChange, int attackChange, int defenseChange) {
        health += healthChange;
        attack += attackChange;
        defense += defenseChange;
    }

    int getHealth() const {
        return health;
    }

    void receiveAttack(int damage) {
        int damageTaken = damage - defense;
        if (damageTaken > 0) {
            modifyStats(-damageTaken, 0, 0);
            cout << "You were attacked and took " << damageTaken << " damage!" << endl;
        } else {
            cout << "You were attacked, but your defense prevented any damage." << endl;
        }
    }
};

class Item {
protected:
    string name;
    int value;

public:
    Item(const string& name, int value) : name(name), value(value) {}

    virtual void use(Character& character) const = 0;
    virtual string getType() const = 0;

    virtual ~Item() {}

    const string& getName() const {
        return name;
    }

    int getValue() const {
        return value;
    }
};

class Weapon : public Item {
private:
    int damage;

public:
    Weapon(const string& name, int damage) : Item(name, damage), damage(damage) {}

    void use(Character& character) const {
        character.modifyStats(0, damage, 0);
        cout << "You attack with " << getName() << " and deal " << damage << " damage!" << endl;
    }

    string getType() const {
        return "Weapon";
    }
};

class Armor : public Item {
private:
    int defense;

public:
    Armor(const string& name, int defense) : Item(name, defense), defense(defense) {}

    void use(Character& character) const {
        character.modifyStats(0, 0, defense);
        cout << "You equip " << getName() << " and gain " << defense << " defense!" << endl;
    }

    string getType() const {
        return "Armor";
    }
};

class Consumable : public Item {
private:
    int healingPower;

public:
    Consumable(const string& name, int healingPower) : Item(name, healingPower), healingPower(healingPower) {}

    void use(Character& character) const {
        character.modifyStats(healingPower, 0, 0);
        cout << "You use " << getName() << " and gain " << healingPower << " health!" << endl;
    }

    string getType() const {
        return "Consumable";
    }
};

void addItemToInventory(vector<Item*>& inventory, Item* item) {
    inventory.push_back(item);
}

void displayInventory(const vector<Item*>& inventory) {
    cout << setw(30) << setfill(' ') << "Inventory" << " " << setfill(' ') << endl;
    cout << setw(56) << setfill('-') << " " << setfill(' ') << endl;
    cout << left << setw(11) << "Item No." << setw(15) << "Item" << setw(15) << "Type" << setw(12) << "Value" << endl;
    for (size_t i = 0; i < inventory.size(); ++i) {
        cout << left << setw(11) << i + 1 << setw(15) << inventory[i]->getName()
             << setw(15) << inventory[i]->getType() << setw(12) << inventory[i]->getValue() << endl;
    }
    cout << setw(56) << setfill('-') << " " << setfill(' ') << endl;
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    Character character(100, 10, 5);
    int enemyHealth = 100;  
    vector<Item*> inventory;

    addItemToInventory(inventory, new Weapon("Sword", 20));
    addItemToInventory(inventory, new Weapon("Axe", 30));
    addItemToInventory(inventory, new Armor("Plate Mail", 30));
    addItemToInventory(inventory, new Armor("Chainmail", 20));
    addItemToInventory(inventory, new Consumable("Health Potion", 50));
    addItemToInventory(inventory, new Consumable("Mana Potion", 30));
    
    cout << setw(56) << setfill('-') << " " << setfill(' ') << endl;
    cout << setw(30) << setfill(' ') << "GAME START" << " " << setfill(' ') << endl;
    cout << setw(56) << setfill('-') << " " << setfill(' ') << endl;

    while (character.getHealth() > 0 && enemyHealth > 0) {
        character.displayStats();
        cout << setw(56) << setfill('-') << " " << setfill(' ') << endl;

        displayInventory(inventory);
        int enemyAttack = rand() % 21 + 20;  
        cout << setw(56) << setfill('-') << " " << setfill(' ') << endl;
        cout << "Enemy Health: " << enemyHealth << endl;
        cout << setw(56) << setfill('-') << " " << setfill(' ') << endl;

        int itemChoice;
        cout << "Enter the number of the item you want to use (or enter 0 to quit): ";
        cin >> itemChoice;
        cout << setw(56) << setfill('-') << " " << setfill(' ') << endl;

        if (itemChoice == 0) {
            cout << "You quit the game." << endl;
            break;
        }

        if (itemChoice >= 1 && itemChoice <= inventory.size()) {
            if (inventory[itemChoice - 1]->getType() == "Weapon") {
                inventory[itemChoice - 1]->use(character);
                enemyHealth -= inventory[itemChoice - 1]->getValue();

                enemyAttack = rand() % 21 + 20;  
                character.receiveAttack(enemyAttack);

                cout << setw(56) << setfill('-') << " " << setfill(' ') << endl;
            } else if (inventory[itemChoice - 1]->getType() == "Armor") {
                inventory[itemChoice - 1]->use(character);
                cout << setw(56) << setfill('-') << " " << setfill(' ') << endl;
            } else {
                inventory[itemChoice - 1]->use(character);
                cout << setw(56) << setfill('-') << " " << setfill(' ') << endl;
            }
        } else {
            cout << "Invalid choice." << endl;
        }
    }

    if (character.getHealth() <= 0) {
        cout << setw(56) << setfill('-') << " " << setfill(' ') << endl;
        cout << setw(30) << setfill(' ') << "You lost! Your health reached 0." << " " << setfill(' ') << endl;
    } else if (enemyHealth <= 0) {
        cout << setw(56) << setfill('-') << " " << setfill(' ') << endl;
        cout << setw(30) << setfill(' ') << "You won! The enemy's health reached 0." << " " << setfill(' ') << endl;
    }
    cout << setw(56) << setfill('-') << " " << setfill(' ') << endl;
    cout << setw(56) << setfill(' ') << "GAME OVER" << " " << setfill(' ') << endl;
    cout << setw(56) << setfill('-') << " " << setfill(' ') << endl;
 
    for (Item* item : inventory) {
        delete item;
    }

    return 0;
}
