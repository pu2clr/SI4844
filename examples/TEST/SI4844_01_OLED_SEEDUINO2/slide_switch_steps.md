
Documentation - AN602


3.4. Si4822/26/40/44 Application Circuits: Slide Switch Select Band and
Use Default Band Property
Figure 7shows an application circuit that uses a slide switch for band selection. In this example, the host MCU
cannot change those band properties already fixed in the default band definition, as stated in section 2.1, it can
only define the band properties which are not fixed in the default band definition, such as softmute property, etc.
For more details, refer to “AN610: Si48xx ATDD Programming Guide”. The two key points to ensure
Si4822/26/40/44 works properly are as follows:
1. Add pull-up resistor R42 of 10 k to pin 1 LNA_EN.
2. Pin 5 BAND is connected to slide switch.


Documentation - AN610

Steps to control the device with  "ATDD device detects the band" mode configuration

5.1.1. Power-On Sequence

Step 1: Reset and Prepare the Device
    1.  The system controller resets the ATDD device and releases the  RSTb  pin.
    2.  The system controller waits until the first  IRQ  is received, indicating that the ATDD device is ready to receive commands.

Step 2: Obtain Device Status
    3. The system controller sends the  ATDD_GET_STATUS  command to retrieve the ATDD device status.
    -  The system controller must check the  BCFG0  bit in the response to determine if the ATDD device or the system controller is responsible for band detection and selection.
    -  If  BCFG0 = 0 , the ATDD device detects the band.
    -  Note: The  HOSTPWRUP  bit is always set at this stage, prompting the system controller to issue the  ATDD_POWER_UP  command.
    -  Band and tune information are  not available  at this point.

Step 3: Power Up the Device
    4. The system controller issues the  ATDD_POWER_UP  command with the following parameters:
    -  Initial Power-Up:
    -  Set  ARG1 bit[5:0] BANDIDX = 0  (since the host doesn’t know the band number).
    -  Crystal Configuration:
    -  Set  ARG1 bit[7] XOSEN = 1  if using a 32.768 kHz crystal.
    -  Set  ARG1 bit[6] XOWAIT = 0  for a standard 600 ms crystallization time, or  XOWAIT = 1  for an extended 900 ms.
    -  Optional Parameters (for Custom Band Configuration):
    -  ARG2, ARG3:  Set if the new lower band frequency differs from the default.
    -  ARG4, ARG5:  Set if the new upper band frequency differs from the default.
    -  ARG6:  Set if the new channel spacing differs (AM only).
    -  ARG7:  Include for new features (Si4827 and Si4844B).

Step 4: Confirm Band Detection
    5. The system controller waits for another  IRQ  indicating that the valid band has been detected.

Step 5: Retrieve Updated Status
    6. The system controller sends the  ATDD_GET_STATUS  command to obtain the latest status:
    -  If  REPLY0 bit[4] INFORDY = 1  (band information ready):
    -  Save the band index in  REPLY1 bit[5:0] BANDIDX  for future use.
    -  Check the following for any required host actions:
    -  HOSTRST (REPLY0 bit[7])  – Reset ATDD if switching across different band modes.
    -  HOSTPWRUP (REPLY0 bit[6])  – Issue  ATDD_POWER_UP  again if switching within the same band mode.

    Band Switching Actions:
    -  Same Band Mode:
    -  Reissue the  ATDD_POWER_UP  command with the known band index.
    -  Different Band Mode:
    -  Reset the ATDD device, wait for IRQ, and issue  ATDD_POWER_UP  again with the known band index.

Step 6: Frequency Detection
    7. The system controller waits for another  IRQ  indicating that the tune wheel frequency is ready.

Step 7: Display Updated Status
    8. The system controller issues the  ATDD_GET_STATUS  command to display the latest information:
    -  If  REPLY0 bit[4] INFORDY = 1  (information ready):
    -  Display the band mode, station, and stereo states.
    -  The tune frequency is ready when the combination of  REPLY2  and  REPLY3  results in a non-zero 4-digit BCD value.
    -  Always save the band index  REPLY1 bit[5:0] BANDIDX  for later use.

Optional Audio Configuration
    9. The system controller can optionally send the  ATDD_AUDIO_MODE  command to select an audio output mode:
    -  AUDIO_MODE Bits (Choose One):
    -  (0) Digital volume mode
    -  (1) Bass/treble mode
    -  (2) Bass/treble + volume control (mix mode 1)
    -  (3) Bass/treble + volume control (mix mode 2)
    -  Additional Settings:
        -  ADJPT_ATTN:  Attenuates audio output by 2 dB for adjacent points (100 kHz).
        -  ADJPT_STEO:  Enables stereo indicator and separation at adjacent points (100 kHz).
        -  FM_MONO:  Forces FM audio output to mono or stereo.

Optional Property Adjustment
    10. The system controller can optionally issue the  SET_PROPERTY  command to adjust:
    -   Receive volume
    -   Bass/treble level
    -   FM de-emphasis
    -   Soft-mute
    -   Banding properties (overrides default settings).


    Here is the improved and organized version of the text:

5.1.2. Band Switch Sequence

Step 1: Band Change Detection
    1. The ATDD device detects a band change and interrupts the system controller when the user switches the band.

Step 2: IRQ Handling and Status Update
    2. The system controller waits until any  IRQ  is received.
    3. The system controller issues the  ATDD_GET_STATUS  command to obtain the latest status:
      -  If REPLY0 bit[4] INFORDY = 1  (band information ready):
      - The host must save the band index in  REPLY1 bit[5:0] BANDIDX  for future use.
      - The host checks the following for required actions:
      -  REPLY0 bit[7] HOSTRST  – Reset ATDD if switching between different band modes.
      -  REPLY0 bit[6] HOSTPOWERUP  – Issue the  ATDD_POWER_UP  command again if the band switch is within the same mode.
      -  Band Switching Actions:
      -  Same Band Mode (HOSTPWRUP = 1):
      - Reissue the  ATDD_POWER_UP  command with the known band index to switch to the correct band.
      -  Different Band Mode (HOSTRST = 1):
      - Reset the ATDD device, wait for IRQ, and issue the  ATDD_POWER_UP  command with the known band index.

Step 3: Frequency Detection
    4. The system controller waits until another  IRQ  is received, indicating that the tune wheel frequency is ready.

Step 4: Display Updated Status
    5. The system controller issues the  ATDD_GET_STATUS  command to display the latest status:
       -  If REPLY0 bit[4] INFORDY = 1  (information ready):
       - Display the band mode, station, and stereo states.
       - The tune frequency is ready when the combined frequency of  REPLY2  and  REPLY3  is non-zero (4-digit BCD value).
       - The host must always save the band index  REPLY1 bit[5:0] BANDIDX  for later use.

Optional Audio Configuration
    6. The system controller can optionally issue the  ATDD_AUDIO_MODE  command to select one of the following audio output modes (FM example):
       -  (0)  Digital volume mode
       -  (1)  Bass/treble mode
       -  (2)  Bass/treble with volume control (mix mode 1)
       -  (3)  Bass/treble with volume control (mix mode 2)

Optional Property Adjustment
    7. Optionally, the system controller can issue the  SET_PROPERTY  command to adjust the following parameters (FM example):
       - Receive volume
       - Bass/treble level
       - FM de-emphasis
       - Soft-mute
       - Banding properties (overrides default settings).


    References:
    Si48XX ATDD PROGRAMMING GUIDE - AN610 (Pages 4, 7 and 8)
