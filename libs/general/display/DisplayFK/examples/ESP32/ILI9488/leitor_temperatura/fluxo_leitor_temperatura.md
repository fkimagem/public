# Fluxo do Código `leitor_temperatura.ino`

Este diagrama mostra o fluxo principal do código, a interação dos widgets e a navegação entre telas.

```mermaid
flowchart TD
    A["setup()"] --> B["Inicializa Serial, SPI, Display, Touch, Sensores, GPIOs"]
    B --> C["WidgetBase::loadScreen = screen0"]
    C --> D["myDisplay.createTask()"]
    D --> E["Cria e inicia xTimerLeitura (callback cb_LerSensor)"]
    E --> F["loop()"]
    F -->|"podeLerSensor == true"| G["Lê sensores de temperatura"]
    G --> H["Atualiza widgets: LineChart, Gauge, Label, CircularBar"]
    H --> I["myDisplay.finishTransaction()"]
    I --> J["Verifica temperatura alta"]
    J -->|"Se >= maxTempAllowed"| K["Desativa toggle/relé 1, desabilita controle manual"]
    J -->|"Senão"| L["Mantém controle manual"]
    F --> M["delay(10)"]
    
    subgraph Widgets
        N["SpinBox"] --> O["spinbox0sb_cb() → Atualiza maxTempAllowed"]
        P["ToggleButton 0"] --> Q["tg10tgb_cb() → LED 1"]
        R["ToggleButton 1"] --> S["tg21tgb_cb() → LED 2"]
        T["ToggleButton 2"] --> U["tg32tgb_cb() → LED 3"]
        V["ToggleButton 3"] --> W["rele13tgb_cb() → Relé 1"]
        X["ToggleButton 4"] --> Y["rele24tgb_cb() → Relé 2"]
        Z["Image icoRight"] --> AA["icoRight0_cb() → WidgetBase::loadScreen = screen1"]
        AB["Image icoLeft"] --> AC["icoLeft1_cb() → WidgetBase::loadScreen = screen0"]
    end
    
    F -.-> Widgets
    
    subgraph Screens
        AD["screen0() → Tela principal"]
        AE["screen1() → Tela de configuração"]
    end
    
    C --> AD
    AA --> AE
    AC --> AD
    
    style A fill:#e0e0e0,stroke:#333,stroke-width:2px
    style F fill:#e0e0e0,stroke:#333,stroke-width:2px
    style AD fill:#b3e6b3,stroke:#333,stroke-width:2px
    style AE fill:#b3e6b3,stroke:#333,stroke-width:2px
    style Widgets fill:#f9f,stroke:#333,stroke-width:1px
    style Screens fill:#bbf,stroke:#333,stroke-width:1px
```

## Legenda
- **setup()**: Inicialização de hardware, display, sensores e widgets.
- **loop()**: Leitura periódica dos sensores, atualização dos widgets e controle dos relés.
- **Widgets**: Callbacks dos botões, spinbox e imagens.
- **Screens**: Funções de desenho das telas principal e de configuração.
- **Navegação**: As imagens permitem alternar entre as telas. 