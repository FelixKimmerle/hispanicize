import random

# Example sentences - you can expand this list
sentences = [
    "Wie spät ist es?",
    "Das ist unglaublich!",
    "Können Sie mir helfen?",
    "Ich bin so aufgeregt!",
    "Wo ist der nächste Supermarkt?",
    "Das war ein großer Fehler!",
    "Glaubst du das wirklich?",
    "Das ist eine tolle Nachricht!",
    "Möchtest du heute Abend ausgehen?",
    "Das kann nicht wahr sein!",
    # Add more sentences as needed
]

# Function to generate a random sentence
def generate_sentence():
    return random.choice(sentences)

# Size of the file in bytes (10 MB)
size = 10 * 1024 * 1024 

with open("german_text.txt", "w", encoding='utf-8') as file:
    while file.tell() < size:
        file.write(generate_sentence() + " ")

print("File generated.")