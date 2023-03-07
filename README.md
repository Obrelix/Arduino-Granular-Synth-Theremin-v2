# Arduino-Granular-Synth-Theremin-v2
An Arduino based projects that utilizes an Arduino Mega 2560 board, 5 10k ohms potentiometers, and 2 IR proximity sensors as inputs.
Based on Peter Knight's Arduino Granular Synth project.

A granular synthesizer is a type of digital synthesizer that produces complex, textured sounds by dividing sound waves into tiny "grains" and manipulating them individually. 

You can explore a vast range of sound textures and manipulate them in real-time. This project is ideal for anyone interested in exploring the boundaries of sound synthesis, especially musicians, sound designers, and electronic hobbyists. With its intuitive interface and customizable controls, its provides an accessible way to experiment with sound and create unique compositions. Whether you're a seasoned pro or a curious beginner, this granular synthesizer is a great way to push the boundaries of sound synthesis and explore the infinite possibilities of sound.

## Bill of Materials
[Arduino Mega 2560 R3](https://www.aliexpress.com/item/1005001621770748.html?spm=a2g0o.productlist.main.3.9df63b32MAAJ7C&algo_pvid=d85cda44-e8ad-4844-9c00-23b1350aadd8&algo_exp_id=d85cda44-e8ad-4844-9c00-23b1350aadd8-1&pdp_ext_f=%7B%22sku_id%22%3A%2212000016846557522%22%7D&pdp_npi=3%40dis%21EUR%2115.23%2112.8%21%21%21%21%21%4021227f7e16782243609872881d071a%2112000016846557522%21sea%21GR%212426965530&curPageLogUid=gQeB4oiq5ITT) x 1

[IR Proximity Sensor (GP2Y0A41SK0f)](https://www.aliexpress.com/item/1005003745155535.html?spm=a2g0o.productlist.main.3.467e430coeVMeg&algo_pvid=a4a0c21b-7e7d-46ef-aa87-3333affe9af9&algo_exp_id=a4a0c21b-7e7d-46ef-aa87-3333affe9af9-1&pdp_ext_f=%7B%22sku_id%22%3A%2212000027016028007%22%7D&pdp_npi=3%40dis%21EUR%214.69%214.45%21%21%21%21%21%40211be17616782247204027213d0714%2112000027016028007%21sea%21GR%212426965530&curPageLogUid=kbcrpDdIagpX) x 2

[Linear Potentiometer 10Κ Ω](https://www.aliexpress.com/item/1005003390403815.html?spm=a2g0o.productlist.main.1.5766237eLunEsj&algo_pvid=d5fbef94-0313-4549-bd29-d5eb26e93311&algo_exp_id=d5fbef94-0313-4549-bd29-d5eb26e93311-0&pdp_ext_f=%7B%22sku_id%22%3A%2212000027422498462%22%7D&pdp_npi=3%40dis%21EUR%210.55%210.44%21%21%21%21%21%40212244c416782234593304413d0728%2112000027422498462%21sea%21GR%212426965530&curPageLogUid=Gtr6hNtFdZkX) x 5

[Toggle Switch (On-Off-On)](https://www.aliexpress.com/item/1005002378931076.html?spm=a2g0o.productlist.main.1.74735889zaA68T&algo_pvid=40cfde46-7e5d-4071-88bd-525644323b71&algo_exp_id=40cfde46-7e5d-4071-88bd-525644323b71-0&pdp_ext_f=%7B%22sku_id%22%3A%2212000020425983750%22%7D&pdp_npi=3%40dis%21EUR%211.31%211.31%21%21%21%21%21%402102160416782240945356879d06f3%2112000020425983750%21sea%21GR%212426965530&curPageLogUid=Oz5lRMOGnAdn) x 1

[Momentary Push Button](https://www.aliexpress.com/item/4000164264475.html?spm=a2g0o.productlist.main.3.510a169d7rGM2i&algo_pvid=0cb51cc9-73c5-4a3f-b947-56bfd0201f6d&algo_exp_id=0cb51cc9-73c5-4a3f-b947-56bfd0201f6d-1&pdp_ext_f=%7B%22sku_id%22%3A%2210000000842680477%22%7D&pdp_npi=3%40dis%21EUR%210.91%210.63%21%21%21%21%21%40211bd3cb16782242073012916d0709%2110000000842680477%21sea%21GR%212426965530&curPageLogUid=P7jq3e1m1H76) x 1

[LED 5mm](https://www.aliexpress.com/item/32731407139.html?spm=a2g0o.productlist.main.11.6afb810cbvFGrs&algo_pvid=eb7c42ea-4f37-476f-a66e-5fc38ed448e5&algo_exp_id=eb7c42ea-4f37-476f-a66e-5fc38ed448e5-5&pdp_ext_f=%7B%22sku_id%22%3A%2212000029313383881%22%7D&pdp_npi=3%40dis%21EUR%211.07%211.02%21%21%21%21%21%40211bf12316782248072498467d06f7%2112000029313383881%21sea%21GR%212426965530&curPageLogUid=5y5snpOdm24a) x 3

[Round Rocker Switch 3Pin red light](https://www.aliexpress.com/item/4000896270246.html?spm=a2g0o.productlist.main.11.4d6d403bJEnoBo&algo_pvid=d1c351cc-111c-420b-bcd3-603eb4f63b21&algo_exp_id=d1c351cc-111c-420b-bcd3-603eb4f63b21-5&pdp_ext_f=%7B%22sku_id%22%3A%2210000010466812197%22%7D&pdp_npi=3%40dis%21EUR%210.5%210.41%21%21%21%21%21%40211bf3f716782249922173050d06c0%2110000010466812197%21sea%21GR%212426965530&curPageLogUid=gmMSond9KilJ) x 1

[DC Power Jack 5.5 x 2.1 mm](https://www.aliexpress.com/item/33024078085.html?spm=a2g0o.productlist.main.51.6df3660fbpNgKG&algo_pvid=5af69a65-9abc-4fcb-a15f-95d8b47c9e2f&algo_exp_id=5af69a65-9abc-4fcb-a15f-95d8b47c9e2f-25&pdp_ext_f=%7B%22sku_id%22%3A%2210000000556133753%22%7D&pdp_npi=3%40dis%21EUR%211.32%210.99%21%21%21%21%21%40211bc2a016782251687131785d06f3%2110000000556133753%21sea%21GR%212426965530&curPageLogUid=aXhfDMm5mvT2) x 1

[2 Pole Mono 3.5mm audio jack](https://www.aliexpress.com/item/4000169463441.html?spm=a2g0o.productlist.main.13.66c82190XV6W9C&algo_pvid=67135418-f947-49d8-b7b2-0f11f5e2d239&algo_exp_id=67135418-f947-49d8-b7b2-0f11f5e2d239-6&pdp_ext_f=%7B%22sku_id%22%3A%2210000000576772549%22%7D&pdp_npi=3%40dis%21EUR%211.12%211.12%21%21%21%21%21%4021227e5116782253661633339d070b%2110000000576772549%21sea%21GR%212426965530&curPageLogUid=wT781DdJvepn) x 1
## Video	
 [![Watch the video](https://i.imgur.com/xYwKKR7.png)](https://youtu.be/2OcyxWDMjQo)
## Images	
 ![alt text](https://github.com/Obrelix/Arduino-Granular-Synth-Theremin-v2/blob/main/Images/Granular_Synth_Theremin_Top.png?raw=true)
 ![alt text](https://github.com/Obrelix/Arduino-Granular-Synth-Theremin-v2/blob/main/Images/Granular_Synth_Theremin2.png?raw=true)
 ![alt text](https://github.com/Obrelix/Arduino-Granular-Synth-Theremin-v2/blob/main/Images/Granular_Synth_Theremin.png?raw=true)
 ![alt text](https://github.com/Obrelix/Arduino-Granular-Synth-Theremin-v2/blob/main/Images/Granular_Synth_Theremin_Bottom.png?raw=true)
 ![alt text](https://github.com/Obrelix/Arduino-Granular-Synth-Theremin-v2/blob/main/Images/Arduino_Granular_Synth_Theremin_bb.png?raw=true)
