@echo off
REM Verifica se foi informado um parâmetro
IF "%~1"=="" (
    echo [ERRO] Informe o nome desejado.
    echo Exemplo de uso: %0 basicShapes
    goto :EOF
)

REM Cria a pasta com o nome fornecido
mkdir "%~1"

REM Cria o arquivo .ino dentro da pasta criada
echo // Arquivo Arduino criado automaticamente > "%~1\%~1.ino"
echo // Adicione seu código Arduino aqui >> "%~1\%~1.ino"