## Requirements

- [SFML](https://www.sfml-dev.org/download/)
- [Ollama](https://github.com/ollama/ollama)

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
