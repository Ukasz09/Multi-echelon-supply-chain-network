# 🚛 Multi echelon supply chain network [![License](https://img.shields.io/badge/licence-MIT-blue)](https://choosealicense.com/licenses/mit/) [![Status](https://img.shields.io/badge/status-finished-brightgreen)](https://github.com/Ukasz09/Multi-echelon-supply-chain-network)
 
 >Multi-echelon supply chain network with RandomSearch and Differential Evolution </br>
 
 MSCN (Multi-echelon supply chain network) is solution that suggests the right levels of inventory at each stage of the supply chain by simultaneously optimizing inventory balance across multiple echelons </br>
 
## Solution calculate methods
 - 🔸 Differential evolution (DE) </br>
   - Metaheuristic algorithm, invented by Storn and Price in 1997 </br></br>
   
   "DE optimizes a problem by maintaining a population of candidate solutions and creating new candidate solutions by combining existing ones according to its simple formulae, and then keeping whichever candidate solution has the best score or fitness on the optimization problem at hand" [Wikipedia Page](https://en.wikipedia.org/wiki/Differential_evolution) </br></br>
   
   (In other words, it is a black-box optimization which means finding the minimum or maximum of a function f(x): R^n → R, where we don’t know its analytical form) </br>
 - 🔸 Random Search </br>
 
  ## How to use 
 1. Make instance of MscnProblem
    - a) Use no-arg constructor. Manual set all parameters of problem (via setters)
    - b) Use no-arg constructor. Manual set quantity of inventory, min and max production capacity and call method ```generateInstance``` with seed for random generator
    - c) Open problem from file (Use parametrized MscnProblem constructor. Data for it you get by making instance of MscnFileManager, than using method ```openFile``` with ```READ_ACCES``` (public static const) parameter and finally calling method ```readProblemFromFile```
    </br></br>
    Pattern of files format you can find below 👇 </br>
    
2. Make instance of solution search algorithms (RandomSearch or DiffEvolution)
3. Call ```findBestSolution``` method 

That's it 👍 </br>
You can also:
- calculate profit from solution (by calling  ```getProfit``` method from MscnProblem) 
- save solution to file (by using instance of MscnFileManager (```WRITE_ACCESS``` parameter) with ```saveSolutionToFile``` method

##  Pattern of files format
```FileFormat.pdf``` 
 
___
## 📫 Contact 
Created by <br/>
<a href="https://github.com/Ukasz09" target="_blank"><img src="https://avatars0.githubusercontent.com/u/44710226?s=460&v=4"  width="100px;"></a>
<br/> gajerski.lukasz@gmail.com - feel free to contact me! ✊
