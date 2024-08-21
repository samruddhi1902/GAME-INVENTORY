class Character {
    constructor(health, attack, defense) {
        this.health = health;
        this.attack = attack;
        this.defense = defense;
    }

    modifyStats(healthChange, attackChange, defenseChange) {
        this.health += healthChange;
        this.attack += attackChange;
        this.defense += defenseChange;
        this.updateStats();
    }

    receiveAttack(damage) {
        let damageTaken = damage - this.defense;
        if (damageTaken > 0) {
            this.modifyStats(-damageTaken, 0, 0);
            showMessage(`You were attacked and took ${damageTaken} damage!`);
        } else {
            showMessage("You were attacked, but your defense prevented any damage.");
        }
    }

    updateStats() {
        document.getElementById('health').innerText = `Health: ${this.health}`;
        document.getElementById('attack').innerText = `Attack: ${this.attack}`;
        document.getElementById('defense').innerText = `Defense: ${this.defense}`;
    }
}

class Item {
    constructor(name, value) {
        this.name = name;
        this.value = value;
    }

    use(character) {
        // To be overridden by subclasses
    }

    getType() {
        // To be overridden by subclasses
    }
}

class Weapon extends Item {
    use(character) {
        character.modifyStats(0, this.value, 0);
        showMessage(`You attack with ${this.name} and deal ${this.value} damage!`);
    }

    getType() {
        return "Weapon";
    }
}

class Armor extends Item {
    use(character) {
        character.modifyStats(0, 0, this.value);
        showMessage(`You equip ${this.name} and gain ${this.value} defense!`);
    }

    getType() {
        return "Armor";
    }
}

class Consumable extends Item {
    use(character) {
        character.modifyStats(this.value, 0, 0);
        showMessage(`You use ${this.name} and gain ${this.value} health!`);
    }

    getType() {
        return "Consumable";
    }
}

let character = new Character(100, 10, 5);
let enemyHealth = 100;
let inventory = [];

function addItemToInventory(item) {
    inventory.push(item);
    updateInventory();
}

function updateInventory() {
    let inventoryList = document.getElementById('inventory-list');
    inventoryList.innerHTML = '';
    inventory.forEach((item, index) => {
        let listItem = document.createElement('li');
        listItem.innerText = `${index + 1}. ${item.name} - ${item.getType()} (${item.value})`;
        listItem.onclick = () => selectItem(index);
        inventoryList.appendChild(listItem);
    });
}

let selectedItemIndex = null;

function selectItem(index) {
    selectedItemIndex = index;
    showMessage(`Selected ${inventory[index].name}`);
}

function useSelectedItem() {
    if (selectedItemIndex !== null) {
        let selectedItem = inventory[selectedItemIndex];
        selectedItem.use(character);
        if (selectedItem.getType() === "Weapon") {
            enemyHealth -= selectedItem.value;
            document.getElementById('enemy-health').innerText = enemyHealth;
            if (enemyHealth <= 0) {
                showMessage("You won! The enemy's health reached 0.");
                endGame();
                return;
            }
            let enemyAttack = Math.floor(Math.random() * 21) + 20;
            character.receiveAttack(enemyAttack);
            if (character.health <= 0) {
                showMessage("You lost! Your health reached 0.");
                endGame();
            }
        }
    } else {
        showMessage("No item selected.");
    }
}

function quitGame() {
    showMessage("You quit the game.");
    endGame();
}

function showMessage(message) {
    document.getElementById('game-message').innerText = message;
}

function endGame() {
    document.getElementById('use-item').disabled = true;
    document.getElementById('quit-game').disabled = true;
}

addItemToInventory(new Weapon("Sword", 20));
addItemToInventory(new Weapon("Axe", 30));
addItemToInventory(new Armor("Plate Mail", 30));
addItemToInventory(new Armor("Chainmail", 20));
addItemToInventory(new Consumable("Health Potion", 50));
addItemToInventory(new Consumable("Mana Potion", 30));
