class TreeNode {
    constructor(contact) {
        this.contact = contact;
        this.left = null;
        this.right = null;
    }

    // For serializing the node into JSON (save in localStorage)
    toJSON() {
        return {
            contact: this.contact,
            left: this.left ? this.left.toJSON() : null,
            right: this.right ? this.right.toJSON() : null
        };
    }

    // Static method to deserialize a node from JSON (rebuild tree)
    static fromJSON(json) {
        if (!json) return null;
        const node = new TreeNode(json.contact);
        node.left = TreeNode.fromJSON(json.left);
        node.right = TreeNode.fromJSON(json.right);
        return node;
    }
}

class ContactBST {
    constructor() {
        this.root = null;
    }

    add(contact) {
        const newNode = new TreeNode(contact);
        if (!this.root) {
            this.root = newNode;
        } else {
            this._insertNode(this.root, newNode);
        }
    }

    _insertNode(node, newNode) {
        if (newNode.contact.name < node.contact.name) {
            if (!node.left) node.left = newNode;
            else this._insertNode(node.left, newNode);
        } else if (newNode.contact.name > node.contact.name) {
            if (!node.right) node.right = newNode;
            else this._insertNode(node.right, newNode);
        }
    }

    delete(name) {
        this.root = this._deleteNode(this.root, name);
    }

    _deleteNode(node, name) {
        if (!node) return null;

        if (name < node.contact.name) {
            node.left = this._deleteNode(node.left, name);
            return node;
        } else if (name > node.contact.name) {
            node.right = this._deleteNode(node.right, name);
            return node;
        } else {
            if (!node.left) return node.right;
            if (!node.right) return node.left;

            const minLargerNode = this._findMin(node.right);
            node.contact = minLargerNode.contact;
            node.right = this._deleteNode(node.right, minLargerNode.contact.name);
            return node;
        }
    }

    _findMin(node) {
        while (node.left) node = node.left;
        return node;
    }

    find(name) {
        return this._findNode(this.root, name);
    }

    _findNode(node, name) {
        if (!node) return null;
        if (name === node.contact.name) return node.contact;
        if (name < node.contact.name) return this._findNode(node.left, name);
        return this._findNode(node.right, name);
    }

    inOrderTraversal() {
        const result = [];
        this._inOrder(this.root, result);
        return result;
    }

    _inOrder(node, result) {
        if (node) {
            this._inOrder(node.left, result);
            result.push(node.contact);
            this._inOrder(node.right, result);
        }
    }

    // For serializing the tree to JSON
    toJSON() {
        return this.root ? this.root.toJSON() : null;
    }

    // Static method to deserialize and rebuild the tree
    static fromJSON(json) {
        const bst = new ContactBST();
        bst.root = TreeNode.fromJSON(json);
        return bst;
    }
}

const contactBST = new ContactBST();
let editingContact = null; // Store contact being edited

// Check if today is anyone's birthday and notify
function checkBirthdays() {
    const today = new Date().toISOString().split('T')[0]; // Get today's date in YYYY-MM-DD format
    const contacts = contactBST.inOrderTraversal();

    contacts.forEach(contact => {
        if (contact.birthday === today) {
            // Notify the user about the birthday
            //alert(`Today is ${contact.name}'s birthday! 🎉`);
            // Optionally, highlight the contact in the list (to make it more visual)
            highlightBirthday(contact);
        }
    });
}

// Function to highlight the birthday contact in the contact list
function highlightBirthday(contact) {
    const contactListItems = document.querySelectorAll('#contactList li');
    contactListItems.forEach(item => {
        if (item.querySelector('strong').textContent === contact.name) {
            item.style.backgroundColor = '#f39c12'; // Highlight the item with a yellow background
            item.style.color = '#2c3e50'; // Change the text color for contrast
        }
    });
}
// Load contacts from localStorage
function loadContacts() {
    const storedTree = JSON.parse(localStorage.getItem('contactBST'));
    if (storedTree) {
        // Rebuild the tree from saved structure
        const rebuiltBST = ContactBST.fromJSON(storedTree);
        contactBST.root = rebuiltBST.root;
    }
    displayContacts();
    visualizeTree(); // Visualize tree on page load
    checkBirthdays();
}

// Save contacts to localStorage
function saveContacts() {
    const bstJSON = contactBST.toJSON();
    localStorage.setItem('contactBST', JSON.stringify(bstJSON));
}

// Add or update a contact
function addOrUpdateContact() {
    const name = document.getElementById('name').value.trim();
    const phone = document.getElementById('phone').value.trim();
    const email = document.getElementById('email').value.trim();
    const birthday = document.getElementById('birthday').value;

    if (name && phone && email && birthday) {
        // Only check for duplicates if we're adding a new contact (not editing)
        if (!editingContact && contactBST.find(name)) {
            alert('A contact with this name already exists.');
            return;
        }

        if (editingContact) {
            // Remove the old contact before adding the updated one
            contactBST.delete(editingContact.name);
            editingContact = null;
        }

        // Add or update the contact
        contactBST.add({ name, phone, email, birthday });
        saveContacts();
        displayContacts();
        visualizeTree(); // Re-render the tree visualization
        checkBirthdays(); // Check for birthdays after adding/updating
        alert('Contact added/updated');
        
        // Clear the input fields
        document.getElementById('name').value = '';
        document.getElementById('phone').value = '';
        document.getElementById('email').value = '';
        document.getElementById('birthday').value = '';
    } else {
        alert('Please fill in all fields');
    }
}


// Prepare to edit a contact
function editContact(name) {
    const contact = contactBST.find(name);
    if (contact) {
        document.getElementById('name').value = contact.name;
        document.getElementById('phone').value = contact.phone;
        document.getElementById('email').value = contact.email;
        document.getElementById('birthday').value = contact.birthday;
        editingContact = contact;
    }
}

// Delete a contact
function deleteContact(name) {
    contactBST.delete(name);
    saveContacts();
    displayContacts();
    visualizeTree(); // Re-render the tree visualization
    alert(`Deleted contact: ${name}`);
}

// Search for a contact
function searchContact() {
    const name = document.getElementById('searchName').value.trim();
    const contact = contactBST.find(name);
    if (contact) {
        alert(`Found: ${contact.name}, Phone: ${contact.phone}, Email: ${contact.email}, Birthday: ${contact.birthday}`);
    } else {
        alert('Contact not found');
    }
    document.getElementById('searchName').value = '';
}

// Display all contacts
function displayContacts() {
    const contactList = document.getElementById('contactList');
    contactList.innerHTML = '';
    contactBST.inOrderTraversal().forEach(contact => {
        const li = document.createElement('li');
        li.innerHTML = `<strong>${contact.name}</strong><br>
                        Phone: ${contact.phone}<br>
                        Email: ${contact.email}<br>
                        Birthday: ${contact.birthday}<br>
                        <button onclick="editContact('${contact.name}')">Edit</button>
                        <button onclick="deleteContact('${contact.name}')">Delete</button>`;
        contactList.appendChild(li);
    });
}

// Visualize the BST
function visualizeTree() {
    const canvas = document.getElementById('bstCanvas');
    const ctx = canvas.getContext('2d');
    const width = canvas.width;
    const height = canvas.height;
    ctx.clearRect(0, 0, width, height); // Clear the canvas

    // Helper function to render the tree
    function renderTree(node, x, y, xOffset) {
        if (!node) return;

        // Draw node
        ctx.beginPath();
        ctx.arc(x, y, 20, 0, 2 * Math.PI);
        ctx.fillStyle = '#3498db';
        ctx.fill();
        ctx.strokeStyle = '#2980b9';
        ctx.stroke();
        ctx.fillStyle = 'white';
        ctx.font = '14px Arial';
        ctx.fillText(node.contact.name, x - 10, y + 5);

        // Draw left and right branches
        if (node.left) {
            ctx.beginPath();
            ctx.moveTo(x, y + 20);
            ctx.lineTo(x - xOffset, y + 100);
            ctx.stroke();
            renderTree(node.left, x - xOffset, y + 100, xOffset / 2);
        }

        if (node.right) {
            ctx.beginPath();
            ctx.moveTo(x, y + 20);
            ctx.lineTo(x + xOffset, y + 100);
            ctx.stroke();
            renderTree(node.right, x + xOffset, y + 100, xOffset / 2);
        }
    }

    renderTree(contactBST.root, width / 2, 40, 150); // Start rendering from root node
}
// Delete a contact by name directly from the search input
function deleteContactByName() {
    const name = document.getElementById('searchName').value.trim();
    if (name) {
        const contact = contactBST.find(name);
        if (contact) {
            contactBST.delete(name);
            saveContacts();
            displayContacts();
            visualizeTree(); // Re-render the tree visualization
            alert(`Deleted contact: ${name}`);
        } else {
            alert('Contact not found');
        }
    } else {
        alert('Please enter a contact name');
    }

    // Clear the search field after deletion
    document.getElementById('searchName').value = '';
}

// Load contacts and visualize the tree on page load
window.onload = loadContacts;
