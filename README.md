# :eyeglasses: Smartwatch usando Arduino e Bluetooth
---
## Explicando o projeto

Neste projeto criei um smartwatch usando arduino, onde além de mostrar a hora em tempo real, através de um módulo rtc, mostra também data, temperatura e tem 4 modos diferentes que são exibidos em uma tela tft, sendo eles um analógico, um digital moderno, um que mostra temperatura, data e uma animação na tela e um cronômetro que inicia, pausa e reseta o cronômetro tanto através de um botão físico, quanto pelo bluetooth através de uma aplicação para celular.  

- Recursos previstos  
Foram usados para adquirir conhecimento para o desenvolvimento do projeto, computadores 
dos integrantes do grupo. Já sobre os componentes utilizados e seus valores na 
finalização do produto temos:  
Arduíno NANO: R$ 40,66  
Jumpers: R$10,90  
Módulo RTC DS3231: R$ 31,26  
Display led: R$ 31,00    
Protoboard: R$ 12,26  
Botão: R$ 0,15  
Módulo bluetooth HC-05: R$ 37,90  
Resistor de 1K: R$ 0,08  
Resistor de 2k: R$ 0,08   
Resistor de 220 ohms: R$0,08  
Todos esses componentes foram adquiridos por uma compra na Shopee.  
  
Além disso, para desenvolvimento do código foram usados os softwares Arduino IDE e o IDLE. 
Todos adquiridos gratuitamente, sendo assim, não houve gastos. 

- Detalhamento técnico do projeto  
Os componentes são Arduino NANO, que é uma placa de microcontrolador de código aberto 
para projetos pequenos que controla e processa informações. Modelo RTC ds3231, é um 
módulo de relógio de tempo real de alta precisão, usado para data e hora, e também usado 
para medir temperatura. Mini display, usado para exibir as informações. Jumpers, pequenos 
fios utilizados para a conexão dos componentes com o Arduíno. Módulo Bluetooth HC-05, 
permite a comunicação sem fio entre dispositivos, como Arduino e computadores ou 
smartphones, podendo atuar tanto como mestre (iniciando a conexão) quanto como escravo 
(recebendo a conexão). Resistor, limitar o fluxo de corrente elétrica em um circuito. Botão, é 
um interruptor para abrir ou fechar um circuito elétrico de forma momentânea quando 
pressionado.

- Referencial teórico (subsídio teórico para propositura de ações da extensão)  
Segundo Felicio, Marcelo, and Rosael Castro de Carvalho [1], O módulo Bluetooth HC-05, é 
um dos principais dispositivos de conexão para o Arduino. É barato, encontrado com 
facilidade em distribuidores de eletrônicos que comercializam Arduino, é de fácil uso e 
entendimento de seus requisitos. Também trabalha em modo mestre ou modo escravo. 
(FELICIO, Marcelo; CARVALHO, Rosael Castro de. Plataforma em material reutilizado, 
motorizada, controlada por arduino e aplicativo android. 2019. Trabalho de Conclusão de 
Curso. Universidade Tecnológica Federal do Paraná). [1]
Segundo Querioz [2], o modulo bluetooth HC-05 atua com o protocolo bluetooth na versão 
2.0 + EDR, simplificando a substituição de conexões seriais sem fio, finalidade para 13 qual é 
usado o perfil Serial Port Profile (SPP) do bluetooth. Como pertence a classe dois, que possui 
potência de 2,5mW, tem um alcance aproximado de 10 metros. 
(QUEIROZ, Isadora Santarém de et al. Diário da Saúde: sistema de registro de evoluções 
diárias com comunicação Bluetooth. 2023). [2]

---
## :clipboard: Tecnologias utilizadas
- C++
---

## :ballot_box_with_check: Componentes e Projetos usados como inspiração

Arduino:  
https://images.tcdn.com.br/img/img_prod/650361/placa_nano_v_3_0_r3_atmega328_sem_cabo_usb_compativel_para_arduino_1183_1_20200818203653.jpg  

Módudo bluetooth:  
https://cdn.awsli.com.br/800x800/468/468162/produto/19414325/3--3--7gzcq8s2z7.jpg  

Módulo rtc:  
https://encrypted-tbn0.gstatic.com/shopping?q=tbn:ANd9GcTvZaJvRnb_4GpNBrvK6RAh1_dxYyoDu1aJWUwIWmo2bHDQWgQXmvFTDN4YerRykgDClZEBabYf-dBPsz-o4XnOuiOvpG2MQuuj8ilfM9Be6NGalRXXRNETRvWSdh1mWF1k9vgs5QTH&usqp=CAc  

Tela tft:  
https://cdn.awsli.com.br/2500x2500/468/468162/produto/155007523/9341ee8702.jpg
---


https://www.squids.com.br/arduino/projetos-arduino/projetos-squids/basico/311-projeto-92-criando-um-relogio-digital-com-oled-ssd1306-modulo-ds3231-e-arduino-biblioteca  
https://www.makerhero.com/blog/tutorial-arduino-bluetooth-hc-05-mestre/?srsltid=AfmBOoodZe3S0z8aRLBmb26FKaG28Nff3P2jKRNpK1lrhajO02psHt3z  
https://forum.arduino.cc/t/how-do-you-change-the-baud-rate-of-hc-05/993572/4  

## :camera: Imagens do Projeto  
Modo 1- Digital Moderno  
![smartwatch-modo1](https://github.com/user-attachments/assets/43129a6c-ec14-4533-86e8-b7def043effe)

Modo 2-  Analógico  
![smartwatch-modo2](https://github.com/user-attachments/assets/45702202-a080-45e3-9255-72c686d3eb5e)

Modo 3-  Temperatura com animação de tela  
![smartwatch-modo3](https://github.com/user-attachments/assets/ba88d6e2-23ec-4519-a6f8-7cc01f537c14)

Modo 4-  Cronômetro  
![smartwatch-modo4](https://github.com/user-attachments/assets/c8527c72-f840-49b6-8b57-9d2d3d6e6698)

Executando testes:   
![Imagem do WhatsApp de 2025-06-13 à(s) 21 48 48_cfe6b843](https://github.com/user-attachments/assets/59d48259-9d54-4037-ba7b-1a8e69cf576b)
![Imagem do WhatsApp de 2025-06-13 à(s) 21 48 48_eb22b2b3](https://github.com/user-attachments/assets/8ec5a5e9-c934-44b1-8d07-6d24ece4c78e)
![Imagem do WhatsApp de 2025-06-13 à(s) 21 48 48_f5e93daf](https://github.com/user-attachments/assets/bdc12509-cfbc-43b5-9034-4459c7cb0ac7)

Esquema do circuito:  
![esquena_circuito](https://github.com/user-attachments/assets/73cfa5dd-a372-46d3-bdea-9db2bba5a86f)

Video:  

https://github.com/user-attachments/assets/731890e1-f389-4556-8ee0-4bb37dc713ce  

https://github.com/user-attachments/assets/bcf10f24-c87e-4d39-92e9-8a5473a3d487


## :x: Problemas enfrentados
1- A tela não exibe nada:  
Testei alguns códigos para saber se as conexões da tela no arduino estavam corretas, mas a tela não exibia nada. Perdi um bom tempo até descobrir que o problema era na bibliotaca que não era compatível com o modelo da tela que eu tinha.  
dica 1 - Pegue as especificações da tela que você está usando e busque a biblioteca que é compatível, nos tutoriais que assisti não parecia ter dificuldade, mas quando eu tentei replicar nada acontecia, pois os modelos das telas eram diferentes, então fique atento ao modelo de telo utilizado.  

2- O bluetooth não responde:
Depois de resolver o probllema da tela, agora era o bluetooth que não respondia, neste caso o problema estava no modo at do módulo bluetooth e como executar as configurações que exigiam não apenas o código correto, mas também na troca dos jumpers na hora correta.  
Como eu resolvi o problema:
✅ Passo 1: Com botão no pino "KEY" ou "EN"  
Desligue o módulo HC-05  
Mantenha o botão do pino KEY/EN pressionado  
Ligue o VCC (com o botão ainda pressionado)  
O LED piscará lentamente (1x por 2s) → está em modo AT  

✅ Passo 2: Puxar o pino KEY/EN para HIGH  
Conecte o pino KEY (ou EN) ao 3.3V antes de ligar o VCC  
Abra o Monitor Serial do Arduino  
Use Ambos NL e CR e Baud Rate 9600 ou 38400 (normalmente é 38400 no modo AT)  
carregue o código desejado ou rode o comando AT para configurar o módulo.  

✅ Passo 3: Salvar, desligar e testar no modo normal
Desligue o módulo  
Desconecte o pino KEY/EN (ou solte o botão)  
Religue o VCC normalmente  
O LED agora pisca rápido  
O módulo está de volta ao modo normal (pronto para emparelhar com celular ou Arduino)  

✅ 6. Testar emparelhamento no celular  
Ative o Bluetooth no celular  
Procure pelo módulo (ou o nome que você colocou)  
Senha padrão: 1234 ou a que você definiu  
Pronto! Agora você pode conectar por aplicativos como o Serial Bluetooth Terminal ou o seu app personalizado.  

Eu perdi muito tempo tentando resolver estes e outros problemas, esses foram os principais, então fique atento as especificações dos componentes que você deseja colocar no seu projeto e busque bem a doocumentação de como utiliza-los assim como projetos específicos para melhor entender o que fazer.
