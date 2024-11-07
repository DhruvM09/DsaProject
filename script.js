class Node {
    constructor() {
        this.links = new Array(26).fill(null);
        this.flag = false;
        this.cntEndWith = 0;
        this.cntPrefix = 0;
    }

    containsKey(ch) {
        return this.links[ch.charCodeAt(0) - 'a'.charCodeAt(0)] !== null;
    }

    put(ch, node) {
        this.links[ch.charCodeAt(0) - 'a'.charCodeAt(0)] = node;
    }

    get(ch) {
        return this.links[ch.charCodeAt(0) - 'a'.charCodeAt(0)];
    }

    setEnd() {
        this.flag = true;
    }

    isEnd() {
        return this.flag;
    }

    increaseEnd() {
        this.cntEndWith++;
    }

    increasePrefix() {
        this.cntPrefix++;
    }

    deleteEnd() {
        this.cntEndWith--;
    }

    reducePrefix() {
        this.cntPrefix--;
    }

    getEnd() {
        return this.cntEndWith;
    }

    getPrefix() {
        return this.cntPrefix;
    }
}

class Trie {
    constructor() {
        this.root = new Node();
    }

    insert(word) {
        let node = this.root;
        for (let i = 0; i < word.length; i++) {
            if (!node.containsKey(word[i])) {
                node.put(word[i], new Node());
            }
            node = node.get(word[i]);
            node.increasePrefix();
        }
        node.setEnd();
        node.increaseEnd();
    }

    search(word) {
        let node = this.root;
        for (let i = 0; i < word.length; i++) {
            if (!node.containsKey(word[i])) {
                return false;
            }
            node = node.get(word[i]);
        }
        return node.isEnd();
    }

    startsWith(prefix) {
        let node = this.root;
        for (let i = 0; i < prefix.length; i++) {
            if (!node.containsKey(prefix[i])) {
                return false;
            }
            node = node.get(prefix[i]);
        }
        return true;
    }

    countWordsEqualTo(word) {
        let node = this.root;
        for (let i = 0; i < word.length; i++) {
            if (node.containsKey(word[i])) {
                node = node.get(word[i]);
            } else {
                return 0;
            }
        }
        return node.getEnd();
    }

    countWordsStartWith(prefix) {
        let node = this.root;
        for (let i = 0; i < prefix.length; i++) {
            if (node.containsKey(prefix[i])) {
                node = node.get(prefix[i]);
            } else {
                return 0;
            }
        }
        return node.getPrefix();
    }

    erase(word) {
        let node = this.root;
        for (let i = 0; i < word.length; i++) {
            if (node.containsKey(word[i])) {
                node = node.get(word[i]);
                node.reducePrefix();
            }
        }
        node.deleteEnd();
    }
}

// Initialize the Trie
const trie = new Trie();
const contacts = {};

// Add contact functionality
document.getElementById('addContactForm').addEventListener('submit', function (e) {
    e.preventDefault(); // Prevent form submission
    const name = document.getElementById('contactName').value.trim().toLowerCase(); // Normalize input
    const phone = document.getElementById('contactPhone').value;
    const email = document.getElementById('contactEmail').value;

    if (name) {
        trie.insert(name);
        contacts[name] = { phone, email }; // Add contact to the contacts object
        document.getElementById('addContactMessage').innerText = `Added contact: ${name}`;
        this.reset(); // Clear the form
    }
});

// Search for contact functionality
document.getElementById('searchButton').addEventListener('click', function () {
    const query = document.getElementById('searchInput').value.trim().toLowerCase();
    const result = Object.keys(contacts).filter(name => trie.startsWith(name));
    if (result.length > 0) {
        document.getElementById('searchResult').innerText = `Found: ${result.join(', ')}`;
    } else {
        document.getElementById('searchResult').innerText = 'No contacts found';
    }
});

// Delete contact functionality
document.getElementById('deleteButton').addEventListener('click', function () {
    const name = document.getElementById('deleteInput').value.trim().toLowerCase();
    if (contacts[name]) {
        trie.erase(name);
        delete contacts[name];
        document.getElementById('deleteMessage').innerText = `Deleted contact: ${name}`;
    } else {
        document.getElementById('deleteMessage').innerText = 'Contact not found';
    }
});

// View all contacts functionality
document.getElementById('viewButton').addEventListener('click', function () {
    const contactList = document.getElementById('contactList');
    contactList.innerHTML = ''; // Clear the list
    for (const name in contacts) {
        const li = document.createElement('li');
        li.innerText = `${name}: ${contacts[name].phone} (${contacts[name].email})`;
        contactList.appendChild(li);
    }
});
