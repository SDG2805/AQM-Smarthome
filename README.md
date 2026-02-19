# AQM-Smarthome 
I'm building AQM (Air-Quality-Monitor)-System to help my family's ventilation system and our health!

For this there are 2 main components: The AQMs (5 in total; 1 per main room) & The Software (a mobile App)

# 1. The AQMs:
- I've decided to handwire them, cuz soldering is fun => no pcb but a wiring diagramm:
<img width="963" height="683" alt="AQMSchematics" src="https://github.com/user-attachments/assets/fb6b0da7-a1b0-4366-9c82-e73965734d9b" />

- After wiring up and soldering all of the parts they'll be in one of these 3d-modelled cases:
<img width="873" height="650" alt="Shreyas_AQM_Case_assembled" src="https://github.com/user-attachments/assets/17e828bb-7062-4885-aba8-489f13561b36" />
- It is made from an upper part or the cover:
<img width="1230" height="612" alt="AQM_case_cover" src="https://github.com/user-attachments/assets/ed14dc85-0bed-44a6-bfc4-d870b23e8297" />
- And from an lower part or the main part:
<img width="876" height="592" alt="AQM_case_lowerpartMainpart" src="https://github.com/user-attachments/assets/0604f40e-b009-46ef-8ca8-6626eb8471f9" />
- They'll be held in place with thier design which has has 4 holes in the main part and 4 'legs' in the cover as well as strong/thick rubberbands. 
- Why rubberbands? Well, it's far easier and cheaper than having to use heatset inserts wand screws, in my opinion.

# 2. The Software:
- code for mcu written in the Ardiuno IDE
- Android App through RemoteXY:
  - RemoteXY has drag-and-drop ui editor
  - costs 1.5€ per month (<2$)
  - lets you add infinite amount of devices
  - lets you imput parameters such as Volume of a Room, People inside a Room, Outside temp 
  - gives notification if ventilation is needed and how long  you should ventilate for according to the upper parameters
  - # IT WORKS! (Based of testing with the prototype which has similar hardware to the final products)

# BOM - Bill of Materials:
<img width="1746" height="615" alt="image" src="https://github.com/user-attachments/assets/664423f9-02b0-4653-b7fe-4bb01ced0db1" />

That's pretty much it! Thanks for reading!


