# EE538 Final Project - Summer 2022 - TrojanMap

#### Author: [Boyang Xiao](https://www.linkedin.com/in/boyang-xiao-40b644225/)

- **Email**:		<a href="mailto:boyangxi@usc.edu">boyangxi@usc.edu</a>
- **Github**:	[here](https://github.com/SeanXiaoby)

#### Dev Environment:

- **OS:** MacOS 12.5.1 / Linux
- **Compiler**: [Bazel 5.1.1](https://bazel.build/)
- **Language:** [C++ 11](https://en.cppreference.com/w/cpp/11)

#### Project Info:
- **Team members:**
   - <a href="boyangxi@usc.edu">Boyang Xiao</a>
   - <a href="xihangde@usc.edu">Xihang Deng</a>
- **Instructor:** [*Arash Saifhashemi*](https://www.linkedin.com/in/ourarash/)
- **Video presentation:** 👉 [here](https://youtu.be/X-evqedsNYc)
- **Project instructions:** 👉 [here](https://github.com/snapfinger/Summer22_TrojanMap/blob/main/README.md)

**⚠️ This repo is forked from the [original repo](https://github.com/ee538/summer22-project-XihangDeng), which is course-project-based and not visible to public. All the commits info are recorded in the original repo.**

**⚠️ The algorithms created in this repo are 100% ORIGINAL. Please DO NOT copy to your final project if you happen to take the same course. You will be DETECTED!!!⚠️**

---

## TrojanMap

In this project, we implement a map application software called TrojanMap. The app is programmed in C++ and several algorithms commonly used on map apps are implemented in this project.

<p align="center"><img src="img/TrojanMap.png" alt="Trojan" width="500" /></p>

#### Index of functions/algorithms implemented in this project:
- [Item 1: Auto-complete](https://github.com/SeanXiaoby/TrojanMap_Sean#item-1-autocomplete-the-location-name-phase-1)
- [Item 2: Find coordinates](https://github.com/SeanXiaoby/TrojanMap_Sean#item-2-1-find-the-places-coordinates-in-the-map-phase-1)
- [Item 3: Get all categories](https://github.com/SeanXiaoby/TrojanMap_Sean#item-3-get-all-categories-phase-2)
- [Item 4: Get all locations in a category](https://github.com/SeanXiaoby/TrojanMap_Sean#item-4-get-all-locations-in-a-category-phase-2)
- [Item 5: Get locations using regular expressions](https://github.com/SeanXiaoby/TrojanMap_Sean#item-5-get-locations-using-a-regular-expression-phase-2)
- [Item 6: Find the shortest path](https://github.com/SeanXiaoby/TrojanMap_Sean#item-6-calculateshortestpath-between-two-places-phase-2)
- [Item 7: Cycle detection](https://github.com/SeanXiaoby/TrojanMap_Sean#item-7-cycle-detection-phase-2)
- [Item 8: Toporlogical sorting](https://github.com/SeanXiaoby/TrojanMap_Sean#item-8-topological-sort-phase-2)
- [Item 9: Travelling Trojan problem](https://github.com/SeanXiaoby/TrojanMap_Sean#item-9-the-traveling-trojan-problem-aka-traveling-salesman-phase-3)
- [Item 10: Find nearby](https://github.com/SeanXiaoby/TrojanMap_Sean#item-10-find-nearby-phase-3)


##

## Data Structure

Each point on the map is represented by the class **Node** shown below and defined in [trojanmap.h](src/lib/trojanmap.h).

```cpp
class Node {
 public:
  Node(){};
  Node(const Node &n) {
    id = n.id;
    lat = n.lat;
    lon = n.lon;
    name = n.name;
    neighbors = n.neighbors;
    attributes = n.attributes;
  };
  std::string id;    // A unique id assign to each point
  double lat;        // Latitude
  double lon;        // Longitude
  std::string name;  // Name of the location. E.g. "Bank of America".
  std::vector<std::string>
      neighbors;  // List of the ids of all neighbor points.
  std::unordered_set<std::string>
      attributes;  // List of the attributes of the location.
};
```

---

### External Libraries Installation

For visualization, we use `OpenCV` library. Here are instructions to install OpenCV on your OS:

#### For MacOS Users

Step 1. type the following three lines in your terminal
```shell
$ brew install cmake
$ brew install opencv
$ brew install ncurses
```

Step 2.

Check the installation paths of opencv and ncurses by

```shell
brew info opencv
```

and

```shell
brew info ncurses
```

respectively, and update their paths in the `WORKSPACE` file of your project root directory with the actual installation paths



#### For Ubuntu users
Step 1. 
```shell
$ cd **your project folder**
$ git clone https://github.com/opencv/opencv.git
$ sudo apt-get install cmake libgtk2.0-dev pkg-config
$ sudo apt install libcanberra-gtk-module libcanberra-gtk3-module
$ sudo apt-get install libncurses5-dev libncursesw5-dev
$ cp ubuntu/* ./
```

Step 2. Make sure you set the **path_to_install_folder** to be the absolute path to the **install** folder under opencv when running the following commands

```shell
$ cd opencv/
$ mkdir build install
$ cd build
$ cmake -D CMAKE_INSTALL_PREFIX=**path_to_install_folder** -D BUILD_LIST=core,highgui,imgcodecs,imgproc,videoio ..
$ make install
```

For example, if cloned this repo under `"/Users/ari/github/TrojanMap"`, you should type:

```shell
$ cd opencv/
$ mkdir build install
$ cd build
$ cmake -D CMAKE_INSTALL_PREFIX=/Users/ari/github/TrojanMap/opencv/install -D BUILD_LIST=core,highgui,imgcodecs,imgproc,videoio ..
$ make install
```

## Run the program

For MacOS users, run

```shell
$ bazel run src/main:main
```

For Ubuntu users, run
               
```shell
$ bazel run --cxxopt='-std=c++17' src/main:main
```

If everything is correct, a menu similar to this will show up.

```shell
TrojanMap Menu
**************************************************************
* Enter the function number (1-11) to start:                  
* 1. Autocomplete                                             
* 2. Find the location                                        
* 3. Find all location categories                             
* 4. Get all locations of a category                          
* 5. Get location matching regular expression                 
* 6. CalculateShortestPath                                    
* 7. Cycle Detection                                          
* 8. Topological Sort                                         
* 9. Traveling salesman problem                              
* 10. Find Nearby                                              
* 11. Exit                                                     
**************************************************************
```

## Test The Program

You can use commands below to test the program.

```shell
$ bazel test tests:trojanmap_test_student
$ bazel test tests:trojanmap_test
```
---

## Item 1: Autocomplete The Location Name (Phase 1)

```c++
std::vector<std::string> Autocomplete(std::string name);
```

We consider the names of nodes as the locations. Implement a method to type the partial name of the location and return a list of possible locations with partial name as prefix. Please treat uppercase and lower case as the same character.

Example:

Input: "Chi" \
Output: ["Chick-fil-A", "Chipotle", "Chinese Street Food"]

Example:

```shell
**************************************************************
* 1. Autocomplete                                             
**************************************************************

Please input a partial location:ch
*************************Results******************************
Chinese Street Food
Cheebos Burger
Chick-fil-A
Chase
Chevron 1
Chipotle
Chase Plaza Heliport
Chevron 2
Church of Christ
Chucks Chicken & Waffles
Chevron
**************************************************************
Time taken by function: 2 ms
```

- What is the runtime of your algorithm?<br> 
*Answer:* Our algorithm has a runtime complexity of O(n).<br><br>
  
- (Optional) Can you do it faster than `O(n)`?

## Item 2-1: Find the place's coordinates in the Map (Phase 1)

```c++
std::pair<double, double> GetPosition(std::string name);
```

Given a location name, return the latitude and longitude. There are no duplicated location names. You should mark the given locations on the map. If the location does not exist, return (-1, -1). The algorithm is case-sensitive.

Example:

Input: "Target" \
Output: (34.0257016, -118.2843512)

```shell
**************************************************************
* 2. Find the position                                        
**************************************************************

Please input a location:Target
*************************Results******************************
Latitude: 34.0257 Longitude: -118.284
**************************************************************
Time taken by function: 1 ms
```
Runtime complexity: O(n)

<p align="center"><img src="img/Target.png" alt="Target" width="500"/></p>

## Item 2-2: Check Edit Distance Between Two Location Names (Phase 2)

```c++
int CalculateEditDistance(std::string name1, std::string name2);
```

When entering a location name that does not exist in the map, your program should determine whether the input can be replaced with a "similar name" or not. By "similar names" we mean the names that exist in the map with a *smallest distance* from the original input. 

The distance between two names A and B is the minimum number of operations required to convert A to B. There are 3 operations:
+ Insert a character
+ Delete a character
+ Replace a character

If the exact match is found, it will be shown on the map. Otherwise, the map will show the most similar name by using FindClosestName and print a warning. 

For example, if I type *Rolphs*, I should get a warning like "Did you mean *Ralphs* instead of *Rolphs*?". 

Write a dynamic programming solution to calculate edit distance. 

```c++
std::string FindClosestName(std::string name);
```

Notes:
- Space can be treated like other characters.
- Spell checking part should be case insensitive.

Example:

Input: "Rolphs", "Ralphs" \
Output: 1


In the user interface of your program, you should show the names that have the minimum edit distance to the name that the user entered.

Example:

Input: "Rolphs"\
Output: "Ralphs"

```shell
**************************************************************
* 2. Find the location                                        
**************************************************************

Please input a location:Rolphs
*************************Results******************************
No matched locations.
Did you mean Ralphs instead of Rolphs? [y/n]y
Latitude: 34.0318 Longitude: -118.291
**************************************************************
Time taken by function: 2 ms
```

## Item 3: Get All Categories (Phase 2)

```c++
std::vector<std::string> GetAllCategories();
```

Some of the locations have category types (`attributes` field in `data.csv` file). 

In this section, your program should print all available categories among all existing categories in the map. There should be no duplicates in the output.

Runtime complexity: O(n)

## Item 4: Get All Locations In A Category (Phase 2)

```c++
std::pair<double, double> GetAllLocationsFromCategory(std::string category);
```

In this section if the user entries a category, the program prints all locations that match that category. For example, if there is a category called "Grocery", your program should print all locations that match the "Grocery" category. 

Runtime complexity: O(n)

## Item 5: Get Locations Using A Regular Expression (Phase 2)

```c++
std::pair<double, double> GetLocationRegex(std::regex location);
```

In this section if the user enters a [regular expression](https://en.wikipedia.org/wiki/Regular_expression), they should see all locations that match that regular expression.

Your program should also verify if the input regular expression was correct.


Runtime complexity: O(n)

## Item 6: CalculateShortestPath between two places (Phase 2)

```c++
std::vector<std::string> CalculateShortestPath_Dijkstra(std::string &location1_name,
                                               std::string &location2_name);
std::vector<std::string> CalculateShortestPath_Bellman_Ford(std::string &location1_name,
                                               std::string &location2_name);
```

Given 2 locations A and B, find the best route from A to B. The distance between 2 points is the euclidean distance using latitude and longitude. You should use both Dijkstra algorithm and Bellman-Ford algorithm. Compare the time for the different methods. Show the routes on the map. If there is no path, please return empty vector.

Example:
```
**************************************************************
* 6. CalculateShortestPath                                    
**************************************************************

Please input the start location:Ralphs
Please input the destination:Target
*************************Dijkstra*****************************
*************************Results******************************
"2578244375","4380040154","4380040158","4380040167","6805802087","8410938469","6813416131","7645318201","6813416130","6813416129","123318563","452688940","6816193777","123408705","6816193774","452688933","452688931","123230412","6816193770","6787470576","4015442011","6816193692","6816193693","6816193694","4015377691","544693739","6816193696","6804883323","6807937309","6807937306","6816193698","4015377690","4015377689","122814447","6813416159","6813405266","4015372488","4015372487","6813405229","122719216","6813405232","4015372486","7071032399","4015372485","6813379479","6813379584","6814769289","5237417650",
The distance of the path is:0.927969 miles
**************************************************************
Time taken by function: 39 ms

*************************Bellman_Ford*************************
*************************Results******************************
"2578244375","4380040154","4380040158","4380040167","6805802087","8410938469","6813416131","7645318201","6813416130","6813416129","123318563","452688940","6816193777","123408705","6816193774","452688933","452688931","123230412","6816193770","6787470576","4015442011","6816193692","6816193693","6816193694","4015377691","544693739","6816193696","6804883323","6807937309","6807937306","6816193698","4015377690","4015377689","122814447","6813416159","6813405266","4015372488","4015372487","6813405229","122719216","6813405232","4015372486","7071032399","4015372485","6813379479","6813379584","6814769289","5237417650",
The distance of the path is:0.927969 miles
**************************************************************
Time taken by function: 7084 ms
```
<p align="center"><img src="img/Routing.png" alt="Routing" width="500"/></p>

#### Runtime complexity:
- Dijakstra: O(nlogn)
- Bellman-ford: O(n<sup>2</sup>)

**Runtime samples**:

| Point A to Point B      | Dijkstra | Bellman-Ford |
| -------------------- | ----------- |-------|
| Ralphs -> Target     |  10420 ms      | 8832 ms    |
| Chick-fli-A -> Target|      12537 ms       | 7937 ms       |        
| Pico -> Chick-fil-A | 27944 ms | 11851 ms |
| Pizza King -> Pico | 43314 ms | 14126 ms|
| Arco -> 76        | 40047 ms | 10611 ms |
| 76 -> Ralphs | 36251 ms | 10827 ms |
| Chick-fil-A -> Arco | 28814 ms | 8057 ms |
| Bevvy -> Subway | 37924 ms | 11225 ms |
| Subway -> Pizza King | 31580 ms | 8690 ms |
| Arco -> Subway | 33380 ms | 10726 ms |
| Bevvy -> Arco | 8125 ms | 11001 ms |
| Target -> Ralphs | 16421 ms | 8282 ms |
| 76 -> Target | 22278 ms | 10540 ms |


## Item 7: Cycle Detection (Phase 2)

```c++
bool CycleDetection(std::vector<double> &square);
```

In this section, we use a square-shaped subgraph of the original graph by using four coordinates stored in ```std::vector<double> square```, which follows the order of left, right, upper, and lower bounds. 

Then try to determine if there is a cycle path in the that subgraph.
If it does, return true and report the path of the cycle on the map. Otherwise return false.

**Your report should include at least five examples.**

Runtime complexity: O(n) (n stands for # of points in the square area)

## Item 8: Topological Sort (Phase 2)

```c++
std::vector<std::string> DeliveringTrojan(std::vector<std::string> &location_names,
                                            std::vector<std::vector<std::string>> &dependencies);
```
In this section, we assume that we are using a UAV which means we can fly directly from 1 point to another point. Tommy Trojan got a part-time job from TrojanEats, for which he needs to pick up and deliver food from local restaurants to various location near the campus. Tommy needs to visit a few different location near the campus with certain order, since there are some constraints. For example, he must first get the food from the restaurant before arriving at the delivery point. 

The TrojanEats app will have some instructions about these constraints. So, Tommy asks you to help him figure out the feasible route!

Here we will give you a vector of location names that Tommy needs to visit, and also some dependencies between those locations.


For example, 

```shell
Input: 
location_names = {"Ralphs", "Chick-fil-A", "KFC"}
dependencies = {{"Ralphs","KFC"}, {"Ralphs","Chick-fil-A"}, {"Chick-fil-A", "KFC"}}
```

Here, ```{"Ralphs","KFC"}``` means
that Tommy must go to `Ralphs` prior to `KFC`.

Your output should be:
```shell
Output: Ralphs  -> Chick-fil-A -> KFC
```
Also, we provide ```PlotPointsOrder``` function that can visualize the results on the map. It will plot each location name and also some arrowed lines to demonstrate a feasible route.

If no feasible route exists, you could simply return an empty vector.

Hint:
- You also need to finish ```ReadLocationsFromCSVFile``` and ```ReadDependenciesFromCSVFile``` functions, so you could read and parse data from you own CSV files. We also give two sample CSV files under ```input``` folder, which could be a reference. 
- When it asks you filenames, you need to give the absolute path.
- If you do not have ```ReadLocationsFromCSVFile``` and ```ReadDependenciesFromCSVFile``` functions ready yet, you can just press enter when it asks you filenames. It will call the default locations and dependencies.
- The locations are actually nodes, and the dependencies could be directed edges. You may want to first construct a DAG and then implement topological sort algorithm to get the route.

Note
- **Your report should show several examples of topological sort with at least 5 to 10 nodes.**


Below is an example output of 3 nodes
```shell
*************************Results******************************
Topological Sorting Results:
Ralphs
Chick-fil-A
KFC
**************************************************************
Time taken by function: 2 ms
```
<p align="center"><img src="img/TopologicalSort.png" alt="TSP" width="500"/></p>

In the user interface, we read the locations and dependencies from `topologicalsort_dependencies.csv` and `topologicalsort_locations.csv` to modify your input there.


## Item 9: The Traveling Trojan Problem (AKA Traveling Salesman!) (Phase 3)

In this section, we assume that we are using a UAV which means we can fly directly from 1 point to another point. Given a vector of location ids, assume every location can reach all other locations in the vector (i.e. assume that the vector of location ids is a complete graph).
Find the shortest route that covers all the locations exactly once and goes back to the start point. 

You will need to return the progress to get the shortest route which will then be converted to an animation.  

We will use the following algorithms:

- Brute-force (i.e. generating all permutations, and returning the minimum)
```c++
std::pair<double, std::vector<std::vector<std::string>>> TravelingTrojan_Brute_force(
      std::vector<std::string> location_ids);
```
- Brute-force enhanced with early backtracking
```c++
std::pair<double, std::vector<std::vector<std::string>>> TravelingTrojan_Backtracking(
      std::vector<std::string> location_ids);
```
- [2-opt Heuristic](https://en.wikipedia.org/wiki/2-opt). Also see [this paper](http://cs.indstate.edu/~zeeshan/aman.pdf)
```c++
std::pair<double, std::vector<std::vector<std::string>>> TravelingTrojan_2opt(
      std::vector<std::string> location_ids);
```

We use early backtracking when the current cost is higher than current minimum.

Please report and compare the time spent by these 3 algorithms. 2-opt algorithm may not get the optimal solution. Please show how far your solution is from the optimal solution.

Show the routes on the map. For each intermediate solution, create a new plot. Your final video presentation should include the changes to your solution.

We will randomly select N points in the map and run your program.

```shell
**************************************************************
* 9. Traveling salesman problem                              
**************************************************************

In this task, we will select N random points on the map and you need to find the path to travel these points and back to the start point.

Please input the number of the places:8
"8201681442","6197156485","7786565237","6820972477","6807600525","1832234142","6819144993","1873055949",
Calculating ...
*************************Results******************************
TravelingTrojan_Brute_force
"8201681442","1873055949","6197156485","1832234142","6807600525","6819144993","7786565237","6820972477","8201681442",
The distance of the path is:7.94756 miles
**************************************************************
You could find your animation at src/lib/output.avi.          
Time taken by function: 59 ms

Calculating ...
*************************Results******************************
TravelingTrojan_Backtracking
"8201681442","6820972477","7786565237","6819144993","6807600525","1832234142","6197156485","1873055949","8201681442",
The distance of the path is:7.94756 miles
**************************************************************
You could find your animation at src/lib/output_backtracking.avi.
Time taken by function: 20 ms

Calculating ...
*************************Results******************************
TravelingTrojan_2opt
"8201681442","1873055949","6197156485","1832234142","6807600525","6819144993","7786565237","6820972477","8201681442",
The distance of the path is:7.94756 miles
**************************************************************
You could find your animation at src/lib/output_2opt.avi.     
Time taken by function: 0 ms
```
<p align="center"><img src="img/TSP.png" alt="TSP" width="500"/></p>

<p align="center"><img src="img/output.gif" alt="TSP videos" width="500"/></p>

#### Runtime complexity:
- Brute-force: O(nlogn)
- Backtracking: O(nlogn)
- 2-opt: O(1) (It depends on how many times we set to stop the loop)

**Runtime samples**:

| Number of Points     | Brute-force | Backtracking | 2-opt | 2-opt results vs optimized resutls|
| -------------------- | ----------- |--------------|-------|-----------------------------------|
| 2 ｜ 0 ms | 0 ms | 0 ms | 0 mi |
| 3| 0 ms | 0 ms | 0 ms | 0 mi |
| 4| 0 ms | 0 ms | 0 ms | 0 mi |
| 5| 1 ms| 0 ms| 1 ms| 0 mi |
| 6| 6 ms | 2 ms | 5 ms | 0 mi |
| 7| 42 ms | 8 ms | 75 ms | 0 mi |
| 8| 35 ms|  62 ms|  457 ms| 0 mi |
| 9| 3279 ms| 548 ms | 5796 ms | 0 mi |
| 10| 34956 ms | 2192 ms | 72937 ms | 0 mi |
| 11| TIME-OUT<sup>*</sup> | 3803 ms | TIME-OUT| NULL|
| 12| TIME-OUT|8333 ms  |TIME-OUT| NULL|

***We set 80,000 ms as a maximum time. If it takes longer than that, it will be marked as *TIME-OUT*.**


## Item 10: Find Nearby (Phase 3)

Given an attribute name `C`, a location name `L` and a number `r` and `k`, find at most `k` locations in attribute `C` on the map near `L`(do not include `L`) with the range of `r` and return a vector of string ids. 

The order of locations should be from
nearest to farthest, and you should not include the current location. 

```c++
std::vector<std::string> TrojanMap::FindNearby(std::string attributesName, std::string name, double r, int k);
```

All attributes:
```
'artwork', 'attraction', 'bakery', 'bank', 'bar', 'beauty', 'beverages', 'bicycle', 'bicycle_rental', 'bus_station', 
'cafe', 'car', 'car_repair', 'car_wash', 'charging_station', 'childcare', 'clinic', 'clothes', 'confectionery', 
'convenience', 'copyshop', 'dentist', 'department_store', 'driving_school', 'fabric', 'fast_food', 'food_court', 
'fountain', 'fuel', 'gallery', 'hairdresser', 'hospital', 'hotel', 'library', 'marketplace', 'mobile_phone', 'museum', 
'music', 'optician', 'parcel_locker', 'parking', 'pharmacy', 'place_of_worship', 'police', 'post_office', 
'restaurant', 'school', 'shoe_repair', 'shoes', 
'skate', 'social_facility', 'supermarket', 'theatre', 
'tobacco', 'yes', 'yoga'
```


Please report and compare the time spent by this algorithm and show the points on the map.

```shell
**************************************************************
* 10. Find Nearby                                    
**************************************************************

Please input the attribute:supermarket
Please input the locations:Ralphs
Please input radius r:10
Please input number k:10
*************************Results******************************
Find Nearby Results:
1 Trader Joes
2 Cal Mart Beer & Wine Food Store
3 Food 4 Less
**************************************************************
Time taken by function: 5 ms
```

<p align="center"><img src="img/Nearby.png" alt="Nearby" width="500"/></p>

## Conclusions and lessons we learnt
In this project, we expererienced a complete process of developing and optimizing a C++ application. We implemented several very classical functions that are widely used in every kind of map apps and other maps, including the functions aiming for strings (e.g. Auto-complete, Edit distance etc...) and algorithms that are used for guidances and navigations (e.g. Shortest paths, TSP etc...). The experience that we study, implement and debug these classical algrithms from scratch can be really valuable for us to apply for jobs as Software Engineers or Algorithm Engineers.

Also, we tried to modify the UI and test cases to optimize the app, which is a necessary skill to practice when we are encoutering a real-world project. Xihang and I worked as a team and we parallelly worked on branches in Github and collaberated together to build the whole project repository. After every time of pull and commits, we can have some conflicts of course. But we learnt how to collaberate and coordinate and we solved all the conflicts in the end. This can be a even more useful skill when we collaberate with other team members in the real world.

To conclude, in this project, we managed to build a real-world program with C++ and OpenCV. We put several algorithms into practice, leant the project structures of a C++ program and learnt how to collaberate on Github reposetories.



## Report and Rubrics:

Your final project should be checked into Github. The [README.md](README.md) of your project is your report. 

### Report:

Your REPORT.md file should include four sections:
Please create a new REPORT.md file and write your report there.

1. High-level overview of your design (Use diagrams and pictures for your data structures).
2. Detailed description of each function and its time complexity.
3. Time spent for each function.
4. Discussion, conclusion, and lessons learned.

### Rubrics:

- Item 1 (AutoComplete): 1 point. (Phase 1)
- Item 2-1 (GetPosition): 1 point. (Phase 1)
- Item 2-2 (EditDistance): 10 points. (Phase 2)
- Item 3 (GetAllCategories): 3 Points. (Phase 2)
- Item 4 (GetAllLocationsFromCategory): 5 Points. (Phase 2)
- Item 5 (GetLocationRegex): 5 Points. (Phase 2)
- Item 6 (Shortest path): 15 points. (Phase 2)
   - Bellman-Ford implementation
   - Dijkstra implementation
   - Table/Plot for comparison, and measure and report time spent by two algorithms.
- Item 7 (Cycle detection): 5 points. (Phase 2)
   - Boolean value and draw the cycle if there exists one.
- Item 8 (Topological Sort): 5 points (Phase 2)
   - Check whether there exist a topological sort or not
   - Return the correct order and plot those point on the map
- Item 9 (Traveling Trojan) (Phase 3)
   - Brute-force: 5 points.
   - Brute-force enhanced with early backtracking: 5 points.
   - 2-opt: 10 points.
   - Animated plot: 5 points.
- Item 10 (FindNearby): 5 points. (Phase 3)
   - Return the correct ids and draw the points.
- Creating reasonable unit tests: 10 points.
      - Three different unit tests for each function.
- Video presentation and report: 10 points. (Phase 3)



## Extra credit items: Maximum of 20 points:
   1. Implementation of [3-opt](http://cs.indstate.edu/~zeeshan/aman.pdf): 10 points.
   2. [Genetic algorithm](https://www.geeksforgeeks.org/traveling-salesman-problem-using-genetic-algorithm/) implementation for Traveling Trojan: 10 points
   3. Create dynamic and animated UI using [ncurses](https://en.wikipedia.org/wiki/Ncurses): 10 points
      - Uncomment #define NCURSES in main.cc and mapui.h
      - Create your menu in DynamicPrintMenu().
      - You could check https://github.com/ourarash/ncurses_bazel
      - Please develope your own UI.
      - Example
         <p align="center"><img src="img/ncurses example.gif" alt="example" width="500"/></p>
               
   4. Accurate measurement of your algorithm runtime using [Google Benchmark](https://www.youtube.com/watch?v=9VKR8u9odrA) while sweeping the input size and providing a diagram of how the runtime grows based on the input size: 10 points.

