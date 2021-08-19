# Port de um RTOS

Exemplo de porte de um sistema operacional de tempo real (RTOS) simples para 3 arquiteturas de microcontrolador:
- HCS08;
- CFV1 (Coldfire v1);
- TM4C129 (Tiva C series).

Foi criado um RTOS básico inspirado nas aulas do prof. [Gustavo Denardin](https://www.youtube.com/playlist?list=PLWFuinjdaFEvDGp-zX2uIBw1VQcop2p2E), contendo instalação de tarefa e suporte a níveis prioridades.

O SO é desenvolvido inicialmente para o HCS08, da Freescale, e simulado na IDE *CodeWarrior*, e em seguida é feito o porte para o CFV1, utilizando a mesma IDE. Por último, o porte para o processador Arm M4 do Tiva Connected, o qual utilizou o *Code Composer Studio* como IDE e a biblioteca *Tivaware* para configuração do *master clock*.

## Organização

Os arquivos e projetos estão organizados em 4 pastas.

1) a pasta `RTOS` contém os arquivos do SO: `os.h` e `os.c`, os quais definem a instauração de tarefas e o funcionamento do escalonador;
2) as pastas `micro 1`, `micro 2` e `micro 3` referem-se aos 3 portes, e dentro de cada uma há o projeto e o arquivo de configuração do RTOS, `config.h`;
3) dentro da pasta `RTOS`, há 3 pastas, cada uma referente a um porte, contendo arquivos `port.h`e `port.c` conforme a arquitetura do processador.
