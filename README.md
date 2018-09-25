# Light-Sensor
My first project exploring the process of taking a design from my head to production.

---

This repo is a continuation of my PIC16F887-Familiarization project. In that project, what began with blinking lights morphed into something utilitarian: turn on an LED when the ambient light gets darker than it was. After breadboarding a working prototype with my 44-pin Demo Board, I decided to take it a step further and try manufacturing the prototype as a finished PCB.

There were two parts to consider: hardware and software. First of all, I didn't need to use the 44-pin PIC16F887 in the production boards... I needed to find the PIC with the least amount of bells and whistles that still do what I wanted. I only needed 1 Analog-to-Digital Converter for the light sensor, and 1 output pin for the LED, so I knew I could probably get away with a bottom-of-the-barrel PIC. I went to Microchip's website, sorted price by "lowest first", and went with the first PIC that had an ADC: the PIC10F220. A whopping 6 pins, 16 bytes of SRAM, and 375 bytes of Flash storage. But, with all of the support circuitry built-in, and an 8-bit ADC, all for $0.40... you just can't beat it.

![img](https://i.imgur.com/OFr6U8P.png)

Once the microcontroller selection process was out of the way, I whipped up a KiCAD mockup of my breadboard circuit, mapped to the PIC10F220 pins instead of my demo board. Unlike my prototype, the finished product would have 2 potential power supplies... the battery, and the PICkit2 programmer. In order to isolate them, I used a 3 position switch as the power-supply selector. Left pin went to the battery, right pin went to the J1 programming header, and middle pin went to VCC. That way there was physically no way the two power sources could ever mix.

![img](https://i.imgur.com/aBgVcKx.png)
![img](https://i.imgur.com/hUR1Jec.png)

After the conceptual circuit diagram was made, I had to think about how it should look laid out on an actual board. I wanted the device to be small, not much bigger than the battery. I took the footprint of the battery holder, and made a 3.5cm x 3cm rectangular board outline around it, making sure to offset it slightly for the through-hole resistors I'd need to find a place for. I laid those out first, followed by the programming header, the PIC, decoupling capacitor, LED, programming jumper, and switch. I aligned these components as close to the perimeter of the board as possible, because since the beginning I had a vision of a QR code linking back to my github planted in the middle... acting as a sort of business card. (Hello, btw, if that is how you found this)

![img](https://i.imgur.com/Xi7u9FQ.png)

With everything looking ship-shape, I placed the order for the parts, and followed [JLCPCB's insctructions](https://support.jlcpcb.com/article/44-how-to-export-kicad-pcb-to-gerber-files) for exporting the gerber files correctly, and sent them off to be fabricated.

Here is the finished product:


![img](https://i.imgur.com/r8z0JZU.png)
![img](https://i.imgur.com/XpKLkg6.png)

---

## But that's not all... (Troubleshooting)

All was not completely well, however. The boards were not acting like the prototype. They didn't work immediately when they were turned on... only after the MCLR pin was manually pulled low. This raised some concerning red flags, namely that there could be something wrong electrically. I noticed that the battery holder was not making adequate contact with the battery, causing the microcontroller to randomly reset itself. But it was still exhibiting the same erroneous behavior even after putting a thicker battery in it... I looked to the datasheet for more advice. There seemed to be no major discrepancies between what the Power-On Reset and the MCLR Reset did as far as resetting the microcontroller... so maybe the Power-On Reset routine wasn't happening at all? According to datasheet figure 8-5, if Vdd doesn't reach its maximum within a certain period of time, the microcontroller will not reset properly. So I pulled off the decoupling capacitor, thinking the battery couldn't charge it quickly enough... but the behavior was unchanged. It still needed to be manually reset to work. Then I thought... what if it was just a software bug all along? Looking at my code, I was taking the first sample from the light sensor and treating it as the "baseline" for all other readings to be compared to. But, as I remembered from before, the oscilloscope showed that the output from the photoresistor was extremely jumpy; often it would read much lower than it should. So I added a function to take 255 samples over the course of about half a second, choose the largest value of those samples, and use that as baseline... and boom! It worked perfectly. The microcontroller was just using bad data all along. Nothing was wrong electrically, just a bug.