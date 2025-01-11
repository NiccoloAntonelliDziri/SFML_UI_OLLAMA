## Requirements

- [SFML](https://github.com/SFML/SFML?tab=readme-ov-file#install)
- [Ollama](https://github.com/ollama/ollama)
- g++17 compiler

## Installation

- Pull `granite3-moe` model from Ollama in your terminal using:
  
```
ollama pull granite3-moe
```

- Create the custom AI model from the modelfile provided:

```
ollama create Cecile -f modelfile
```

- Compile and run the app:
```
make run
```
