# SLIDE SWITCH EXAMPLE


The Si48XX IC family (Si4822/26/27/40/44) offers two methods for band selection:  

1. The microcontroller sends commands to the Si48XX to change the band when triggered by the user, typically via a pushbutton.  
2. The user selects the band manually using a mechanical band selector (such as a slide switch or rotary switch), and the Si48XX detects this change and notifies the microcontroller.  

This section focuses on the second method, where a mechanical band selector, consisting of a resistor network (voltage divider), determines the active band at any given time.  


   
## Hardware Configuration**  

Unlike the version where the microcontroller handles band selection, the version where the Si48XX device detects the band via a mechanical selector requires some circuit modifications:  

1. **Building the Band Selector:** The selector must be constructed with the correct resistor values to achieve the desired bands. This step is arguably the most labor-intensive in this approach.  
2. **Connecting the BAND Pin:** In the microcontroller-controlled version, the BAND pin is connected to +VCC. However, in the mechanical selection version, it must be connected to the central pin of the band selector.  

The circuit diagram below illustrates this configuration.  


![Circuit diagram the mechanical band selection](../../extras/KiCad/slide_switch_banda_selection.jpg)



# References: 
  
* Si4844-B DEMO BOARD USER’S GUIDE 
* Si4827 DEMO BOARD USER’S GUIDE
* Si4822/26/27/40/44 ANTENNA, SCHEMATIC, LAYOUT, AND DESIGN GUIDELINES - AN602
    
