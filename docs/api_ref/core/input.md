# **Lotus Input API Reference 🪷**

## **Introduction**  
The **Lotus Input API** is responsible for handling keyboard and mouse input. It provides functionality to detect key presses/releases, mouse movements, button presses, and other user interactions. This API is used to capture input events for processing during the game loop, enabling interactive gameplay and UI experiences.

---

<br>
<br>

<a id="Macros"></a>
# **Macros 🪷**  

### **Reference Sections:** [Structures](#Structures) | [Functions](#Functions)  

```c
#define LOTUS_MBUTTON_MAX_BUTTONS    3
```
> Defines the maximum number of mouse buttons supported (`3` buttons - left, right, middle).  

---

<br>
<br>

<a id="Structures"></a>
# **Structures 🪷**  

### **Reference Sections:** [Macros](#Macros) | [Functions](#Functions)  

#### **`Lotus_Keyboard_Key`** 🪷  

```c
typedef enum Lotus_Keyboard_Key {
    LOTUS_DEFINE_KEY(BACKSPACE, 0x08),
    LOTUS_DEFINE_KEY(ENTER, 0x0D),
    LOTUS_DEFINE_KEY(TAB, 0x09),
    LOTUS_DEFINE_KEY(SHIFT, 0x10),
    LOTUS_DEFINE_KEY(CONTROL, 0x11),
    // More keys omitted for brevity...
    LOTUS_MAX_KEYS
} Lotus_Keyboard_Key;
```

**Description**:  
An enumeration representing all the keyboard keys supported by **Lotus Input**.  
- **BACKSPACE**, **ENTER**, **SHIFT**, **CONTROL**, **F1-F12**, **NUMPAD**, **ESCAPE**, and more.

---

#### **`Lotus_Input_State`** 🪷  

```c
typedef struct Llotus_Input_State {
    sbyte2 mouse[2];
    ubyte keys[256];
    ubyte buttons[LOTUS_MBUTTON_MAX_BUTTONS];
} Llotus_Input_State;
```

**Description**:  
A structure that holds the current input state for the mouse and keyboard.  
- **mouse**: Stores the X, Y position of the mouse.  
- **keys**: Array representing the state of each key (pressed or not).  
- **buttons**: Stores the state of each mouse button.  

---

#### **`Lotus_Input_State`** 🪷  

```c
typedef struct Lotus_Input_State {
    ubyte init;
    Llotus_Input_State current;
    Llotus_Input_State previous;
} Lotus_Input_State;
```

**Description**:  
A structure that holds both the **current** and **previous** states of the input devices (mouse and keyboard). This allows comparison between the current frame and the previous frame to detect state changes (e.g., key presses/releases).  

- **init**: Initialization flag to check if the input state has been initialized.  
- **current**: Holds the current state of the keyboard and mouse.  
- **previous**: Holds the previous state of the keyboard and mouse (for detecting key presses/releases).

---

<br>
<br>

<a id="Functions"></a>
# **Functions 🪷**  

### **Reference Sections:** [Macros](#Macros) | [Structures](#Structures)  

Below is a list of the functions provided in the **Lotus Input API**. Click on any function to jump directly to its documentation.

### **Initialization and Cleanup Functions:**  
### **Key Input Functions:**  
- [lotus_key_is_up](#lotus_key_is_up) | [lotus_key_was_up](#lotus_key_was_up)  
- [lotus_key_is_down](#lotus_key_is_down) | [lotus_key_was_down](#lotus_key_was_down)

### **Mouse Input Functions:**  
- [lotus_mouse_get_position](#lotus_mouse_get_position) | [lotus_mouse_get_last_position](#lotus_mouse_get_last_position)
- [lotus_button_is_up](#lotus_button_is_up) | [lotus_button_was_up](#lotus_button_was_up)  
- [lotus_button_is_down](#lotus_button_is_down) | [lotus_button_was_down](#lotus_button_was_down)

---

<a id="lotus_mouse_get_position"></a>
#### **`lotus_mouse_get_position()`** 🪷  

**Description**:  
Gets the current mouse position in the window.  

**Parameters**:  
- `x` (`sbyte2*`): Pointer to store the current X position.  
- `y` (`sbyte2*`): Pointer to store the current Y position.  

**Return Type**:  
None.  

---

<a id="lotus_mouse_get_last_position"></a>
#### **`lotus_mouse_get_last_position()`** 🪷  

**Description**:  
Gets the previous mouse position before the last update.  

**Parameters**:  
- `x` (`sbyte2*`): Pointer to store the last X position.  
- `y` (`sbyte2*`): Pointer to store the last Y position.  

**Return Type**:  
None.  

---

<a id="lotus_key_is_up"></a>
#### **`lotus_key_is_up()`** 🪷  

**Description**:  
Checks if a specific key is currently **up** (not pressed).  

**Parameters**:  
- `key` (`Lotus_Keyboard_Key`): The key to check.  

**Return Type**:  
`ubyte` - `1` if the key is up, `0` otherwise.  

---

<a id="lotus_key_was_up"></a>
#### **`lotus_key_was_up()`** 🪷  

**Description**:  
Checks if a specific key **was** up in the previous frame.  

**Parameters**:  
- `key` (`Lotus_Keyboard_Key`): The key to check.  

**Return Type**:  
`ubyte` - `1` if the key was up in the previous frame, `0` otherwise.  

---

<a id="lotus_key_is_down"></a>
#### **`lotus_key_is_down()`** 🪷  

**Description**:  
Checks if a specific key is currently **down** (pressed).  

**Parameters**:  
- `key` (`Lotus_Keyboard_Key`): The key to check.  

**Return Type**:  
`ubyte` - `1` if the key is down, `0` otherwise.  

---

<a id="lotus_key_was_down"></a>
#### **`lotus_key_was_down()`** 🪷  

**Description**:  
Checks if a specific key **was** down in the previous frame.  

**Parameters**:  
- `key` (`Lotus_Keyboard_Key`): The key to check.  

**Return Type**:  
`ubyte` - `1` if the key was down in the previous frame, `0` otherwise.  

---

<a id="lotus_button_is_up"></a>
#### **`lotus_button_is_up()`** 🪷  

**Description**:  
Checks if a specific mouse button is currently **up** (not pressed).  

**Parameters**:  
- `button` (`Lotus_Mouse_Button`): The mouse button to check.  

**Return Type**:  
`ubyte` - `1` if the button is up, `0` otherwise.  

---

<a id="lotus_button_was_up"></a>
#### **`lotus_button_was_up()`** 🪷  

**Description**:  
Checks if a specific mouse button **was** up in the previous frame.  

**Parameters**:  
- `button` (`Lotus_Mouse_Button`): The mouse button to check.  

**Return Type**:  
`ubyte` - `1` if the button was up in the previous frame, `0` otherwise.  

---

<a id="lotus_button_is_down"></a>
#### **`lotus_button_is_down()`** 🪷  

**Description**:  
Checks if a specific mouse button is currently **down** (pressed).  

**Parameters**:  
- `button` (`Lotus_Mouse_Button`): The mouse button to check.  

**Return Type**:  
`ubyte` - `1` if the button is down, `0` otherwise.  

---

<a id="lotus_button_was_down"></a>
#### **`lotus_button_was_down()`** 🪷  

**Description**:  
Checks if a specific mouse button **was** down in the previous frame.  

**Parameters**:  
- `button` (`Lotus_Mouse_Button`): The mouse button to check.  

**Return Type**:  
`ubyte` - `1` if the button was down in the previous frame, `0` otherwise.  

---