# **Lotus Event API Reference 🪷**  

## **Introduction**  
The **Lotus Event API** provides an event-driven system for handling various types of engine and user-defined events. It enables **event registration**, **event dispatching**, and **callback management**, offering a flexible way to manage system events such as **keyboard input**, **mouse movements**, and **window resizing**.  

This API follows a **publish-subscribe model**, where events can be pushed to a queue, and registered callbacks can respond dynamically.  

---

<br>
<br>

<a id="Macros"></a>
# **Macros 🪷**  

### **Reference Sections:** [Structures](#Structures) | [Functions](#Functions)  

```c
#define LOTUS_EVENT_MAX     ((1U << 12) - 1)
```
> Defines the maximum number of events that can exist in the event queue at one time (`4095`).  

```c
#define LOTUS_CALLBACK_MAX  (1U << 5)
```
> Defines the maximum number of callbacks that can be registered for an event (`32`).  

---

<br>
<br>

<a id="Structures"></a>
# **Structures 🪷**  

### **Reference Sections:** [Macros](#Macros) | [Functions](#Functions)  

#### **`Lotus_Event_Code`** 🪷  

```c
typedef enum Lotus_Event_Code {
    LOTUS_EVENT_APP_QUIT = 0,
    LOTUS_EVENT_KEY_PRESSED,
    LOTUS_EVENT_KEY_RELEASED,
    LOTUS_EVENT_BUTTON_PRESSED,
    LOTUS_EVENT_BUTTON_RELEASED,
    LOTUS_EVENT_MOUSE_MOVE,
    LOTUS_EVENT_MOUSE_WHEEL,
    LOTUS_EVENT_RESIZE,
    LOTUS_EVENT_CODES,
    LOTUS_EVENT_CODE_MAX = ((1U << 8) - 1)
} Lotus_Event_Code;
```

**Description**:  
An enumeration defining different types of events that can be processed by the **Lotus Event System**.  

- **LOTUS_EVENT_APP_QUIT**: Signals the application should close.  
- **LOTUS_EVENT_KEY_PRESSED**: Triggered when a key is pressed.  
- **LOTUS_EVENT_KEY_RELEASED**: Triggered when a key is released.  
- **LOTUS_EVENT_BUTTON_PRESSED**: Triggered when a mouse button is pressed.  
- **LOTUS_EVENT_BUTTON_RELEASED**: Triggered when a mouse button is released.  
- **LOTUS_EVENT_MOUSE_MOVE**: Triggered when the mouse moves.  
- **LOTUS_EVENT_MOUSE_WHEEL**: Triggered when the mouse wheel is scrolled.  
- **LOTUS_EVENT_RESIZE**: Triggered when the window is resized.  

---

#### **`Lotus_Event`** 🪷  

```c
typedef union Lotus_Event {
    sbyte8 sbyte8[2];
    ubyte8 ubyte8[2];
    f64 f64[2];

    sbyte4 sbyte4[4];
    ubyte4 ubyte4[4];
    f32 f32[4];

    sbyte2 sbyte2[8];
    ubyte2 ubyte2[8];

    sbyte sbyte[16];
    ubyte ubyte[16];
    char c[16];
} Lotus_Event;
```

**Description**:  
A **128-bit** event payload that can store various types of data.  

- Can hold **integers**, **floats**, or **character data**, making it flexible for different event types.  
- Used to pass information between event producers and subscribers.  

---

#### **`Lotus_Event_Callback`** 🪷  

```c
typedef ubyte (*Lotus_Event_Callback)(Lotus_Event data, ubyte2 event_code);
```

**Description**:  
A function pointer type used to define **event callbacks**.  

- **data** (`Lotus_Event`): The event data payload.  
- **event_code** (`ubyte2`): The event type.  
- **Returns** `1` if handled, `0` otherwise.  

---

<br>
<br>

<a id="Functions"></a>
# **Functions 🪷**  

### **Reference Sections:** [Macros](#Macros) | [Structures](#Structures)  

Below is a list of the functions provided in the **Lotus Event API**. Click on any function to jump directly to its documentation.  

### **Event Management Functions:**  
- [register_event](#register_event) | [unregister_event](#unregister_event) | [push_event](#push_event)  

### **Callback Functions:**  
- [register_callback](#register_callback) | [unregister_callback](#unregister_callback)  

---

<a id="register_event"></a>
#### **`register_event()`** 🪷  

**Description**:  
Registers a new event type in the system.  

**Parameters**:  
- `event_code` (`ubyte2`): The event type to register.  

**Return Type**:  
`ubyte` - `1` on success, `0` on failure.  

---

<a id="unregister_event"></a>
#### **`unregister_event()`** 🪷  

**Description**:  
Removes a previously registered event type from the system.  

**Parameters**:  
- `event_code` (`ubyte2`): The event type to unregister.  

**Return Type**:  
`ubyte` - `1` on success, `0` on failure.  

---

<a id="push_event"></a>
#### **`push_event()`** 🪷  

**Description**:  
Pushes an event to the event queue, which will be processed in the next update cycle.  

**Parameters**:  
- `data` (`Lotus_Event`): The event payload.  
- `event_code` (`ubyte2`): The event type.  

**Return Type**:  
`ubyte` - `1` if the event was successfully added, `0` if the queue is full.  

---

<a id="register_callback"></a>
#### **`register_callback()`** 🪷  

**Description**:  
Registers a callback function to be triggered when a specific event occurs.  

**Parameters**:  
- `event_code` (`ubyte2`): The event type to attach the callback to.  
- `callback` (`Lotus_Event_Callback`): The function to be called when the event is fired.  

**Return Type**:  
`ubyte` - `1` on success, `0` if the maximum number of callbacks for this event has been reached.  

---

<a id="unregister_callback"></a>
#### **`unregister_callback()`** 🪷  

**Description**:  
Removes a previously registered callback from an event.  

**Parameters**:  
- `event_code` (`ubyte2`): The event type.  
- `callback` (`Lotus_Event_Callback`): The function to remove.  

**Return Type**:  
`ubyte` - `1` on success, `0` if the callback was not found.  

---