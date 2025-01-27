#include <LiquidCrystal.h>
#include <Keypad.h>

// Setup for the LCD and keypad
LiquidCrystal lcd(A4, A5, A3, A2, A1, A0);
const byte rows = 4;
const byte cols = 3;

char key[rows][cols] = {
  { '1', '2', '3' },
  { '4', '5', '6' },
  { '7', '8', '9' },
  { '*', '0', '#' }
};
byte rowPins[rows] = { 0, 1, 2, 3 };
byte colPins[cols] = { 4, 5, 6 };
Keypad keypad = Keypad(makeKeymap(key), rowPins, colPins, rows, cols);

char password[5] = "0000"; // Default password
int currentPosition = 0;

// Custom Stack Class
class Stack {
  private:
    char data[10];
    int top;

  public:
    Stack() : top(-1) {}

    bool push(char c) {
      if (top < 9) {
        data[++top] = c;
        return true;
      }
      return false;
    }

    bool pop() {
      if (top >= 0) {
        top--;
        return true;
      }
      return false;
    }

    String getString() {
      String result = "";
      for (int i = 0; i <= top; i++) {
        result += data[i];
      }
      return result;
    }

    int size() {
      return top + 1;
    }

    void clear() {
      top = -1;
    }
};

Stack inputStack;

// Functions
void displayscreen();
void unlock();
void incorrect();
void change_password();
void displayInput();

void setup() {
  lcd.begin(16, 2);
  displayscreen();
}

void loop() {
  char code = keypad.getKey();
  if (code != NO_KEY) {
    if (code == '*') {
      inputStack.pop();
    } else if (code == '#') {
      if (inputStack.getString() == password) {
        unlock();
      } else {
        incorrect();
      }
      inputStack.clear();
    } else {
      inputStack.push(code);
    }
    displayInput();
  }
}

void displayscreen() {
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("-ENTER PASS-");
  lcd.setCursor(2, 1);
  lcd.print("|__________|");
}

void unlock() {
  lcd.clear();
  lcd.print("ACCESS GRANTED");
  delay(1000);
  lcd.clear();
  lcd.print("1.CHANGE PASS");
  lcd.setCursor(0, 1);
  lcd.print("2.EXIT");
  while (true) {
    char code = keypad.getKey();
    if (code != NO_KEY) {
      if (code == '1') {
        change_password();
        break;
      } else if (code == '2') {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("EXITING...");
        delay(1000);
        displayscreen();
        break;
      }
      else
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("WRONG INPUT!");
        delay(1000);
        lcd.clear();
        lcd.print("1.CHANGE PASS");
        lcd.setCursor(0, 1);
        lcd.print("2.EXIT");
      }
    }
  }
}

void incorrect() {
  lcd.clear();
  lcd.print("CODE INCORRECT");
  delay(1000);
  displayscreen();
}

void change_password() {
  char new_pass[5] = "----";  // Temporary storage for the new password
  Stack tempStack;           // Stack for entering the new password
  lcd.clear();
  lcd.print("New Password:");
  int currentPosition = 0;

  // Enter new password with backspace support
  while (true) {
    char code = keypad.getKey();
    if (code != NO_KEY) {
      if (code != '#')
      {
        if (code == '*') {
          // Handle backspace
          if (tempStack.pop()) {
            currentPosition--;
          }
        } else if (currentPosition < 4) {
          // Push new character to stack
          tempStack.push(code);
          currentPosition++;
        }
      }
      // Display asterisks for entered characters
      lcd.setCursor(0, 1);
      lcd.print("    ");  // Clear display line
      lcd.setCursor(0, 1);
      for (int i = 0; i < tempStack.size(); i++) {
        lcd.print('*');
      }
      if (tempStack.size() == 4) {
        break;  // Password input complete
      }
    }
  }

  // Store entered password in `new_pass`
  String tempPass = tempStack.getString();
  tempPass.toCharArray(new_pass, 5);

  lcd.clear();
  lcd.print("CONFIRM PASS:");
  tempStack.clear();  // Clear stack for confirmation
  currentPosition = 0;


  // Confirm new password with backspace support
  while (true) {
    char code = keypad.getKey();
    if (code != NO_KEY) {
      if (code != '#')
      {
        if (code == '*') {
          // Handle backspace
          if (tempStack.pop()) {
            currentPosition--;
          }
        } else if (currentPosition < 4) {
          // Push new character to stack
          tempStack.push(code);
          currentPosition++;
        }
      }
      // Display asterisks for entered characters
      lcd.setCursor(0, 1);
      lcd.print("    ");  // Clear display line
      lcd.setCursor(0, 1);
      for (int i = 0; i < tempStack.size(); i++) {
        lcd.print('*');
      }
      if (tempStack.size() == 4) {
        break;  // Confirmation input complete
      }
    }
  }

  // Verify the confirmation password
  if (tempStack.getString() == String(new_pass)) {
    strncpy(password, new_pass, 4);
    password[4] = '\0';
    lcd.clear();
    lcd.print("PASSWORD CHANGED");
  } else {
    lcd.clear();
    lcd.print("MISMATCH!");
  }

  delay(1000);
  displayscreen();
}


void displayInput() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PASSWORD:");
  lcd.setCursor(0, 1);
  for (int i = 0; i < inputStack.size(); i++) {
    lcd.print('*');
  }
}


