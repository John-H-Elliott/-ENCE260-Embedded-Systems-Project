# ENCE260-**Dodge&Dash**-Game

![Logo](Resources/Dodge%20&%20Dash.gif)

---

## How To Play!

---

### **Aim of the Game**

>_You are a ninja and your village is being raided by a enemy clan from the future.  
>You have to dodge their lasers and stay alive as they are try to hit you._

### **Ninja**

>**The Ninja is the character you play**.  
>He starts in the bottom right corner of the screen and you have to can move him by using the navstick.  
>You can tell if the Ninja is hit by a laser as the **Blue LED** on the board will turn off.

### **Smoke Bombs**

>The Ninja has **5 smoke bombs** which will give him time to hide before the lasers fire.  
>To use them you push down on the centre of the navstick.  
>When the Ninja has used all his smokes bombs the top left corner will turn off.

### **Lasers**

>The lasers are what the Ninja is trying to dodge.
>The side of the screen will have LEDS flash. This indicates where the lasers will be coming from.  
>The Ninja will only have a certain amount of time to move between the lasers to dodge them before they are fired and fill up the screen.
>**As time goes on the lasers will get faster and faster.**
---

## How to Compile the Game

---

To run the game you will need University of Canterbury Fun Kits Version-4 ([UCFK4](https://checkout.canterbury.ac.nz/cart.php?action=buy&sku=1000200&source=buy_button "UCFK4 in Store")) and the approate cable ([USB Mini-B](https://checkout.canterbury.ac.nz/cart.php?action=buy&sku=1000201&source=buy_button "Cable in Store")).  

This game is compiled using the **gcc-avr** compiler and needs certain required packages. To install these packages ensure you are running a **Linux** distribution and run the following command in the console.

```Console
sudo apt-get install git gcc-avr binutils-avr avr-libc dfu-programmer
```

If there are problems when compiling you may also need to create a ***udev rule*** in order to allow access to the USB ports. To do so run the following command in the console.

```Console
sudo -i
  echo 'SUBSYSTEM=="usb", ATTRS{idVendor}=="03eb", ATTRS{idProduct}=="2ff0", GROUP="plugdev"' > /etc/udev/rules.d/52-bootloadDFU.rules ; exit
```

From within the same directory as the [Game files](./ "Main Directory"), the following command can be run to compile the game code.

- `make`: This will compile the source code and builds object files based on the **[Makefile](./Makefile)**.

For optional tidiness you can run the following.

- `make clean`: This will remove old object files from the directory.

---

## How to Flash Game onto the Board

---

1. To flash the game onto the board you must have the **UCFK4** connected via the **USB Mini-B** cable to the computer.

2. Then run the `make program` command to compile and flash the game onto the board.

---

## Authors

---

**John Elliott** (_[Jel119](mailto:jel119@uclive.ac.nz "John Elliott UC Email")_)  
**Menghao Zhan** (_[mzh99](mailto:mzh99@uclive.ac.nz "Menghao Zhan UC Email")_)
