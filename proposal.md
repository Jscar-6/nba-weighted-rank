# Project 2a Proposal

**Team Name - Jombert**
**Team Members - Thomas McLaughlin, Jack Scarlett, Hubert Ferro**
**Link - COP3530 Project 2a Proposal**
**Project Title - NBA Single-Game Performance Weighted Rankings**

1. **Problem:** What problem are we trying to solve?
    1. The problem we are trying to solve is dynamically identifying the greatest single-game NBA performances based on historical game logs and custom user-weights for statistical categories.
2. **Motivation:** Why is this a problem?
    1. Different fans have different ideas for what makes a great performance in the game of basketball. Whether it’s a preference of more offensive stats like points and assists or defensive stats like blocks and steals, there are a lot of different things that contribute to someone having a great game. Thus, if a user wanted to point out the performances that were truly the best in their opinion, they may have to look through countless games and data. However, sorting and looking through individual game performances is very computationally expensive, making data structures valuable.
3. **Features**: When do we know that we have solved the problem?
    1. We will know that we have solved the problem when a user can input custom weights for player stats (points, assists, rebounds, steals, blocks, etc.) and receive the top 25 single-game performances that fit those weights. Each of the compared data structures will provide their execution time upon completion of returning the top 25 performances for analysis.
4. **Data**: (Public data set we will be using and the link to the public data set) **or** (Schema of randomly generated data - i.e. what are the different columns in our dataset and the respective datatypes)
    1. We plan on using the Kaggle NBA Dataset with datapoints from 1947-2026. Dataset - <https://www.kaggle.com/datasets/eoinamoore/historical-nba-data-and-player-box-scores?resource=download&select=PlayerStatistics.csv>
    2. We plan on using the player statistic columns for each player in a particular game across the years of individual games (numMinutes, points, assists, rebounds, blocks, steals) and potentially more descriptive columns (fieldGoalsPercentage, threePointersPercentage, etc).
    3. The dataset contains over 100,000 rows of player data across almost 80 seasons of NBA basketball.
5. **Tools:** Programming languages or any tools/frameworks we will be using
    1. We plan on using C++ for the development of the data structures and program logic.
    2. We also plan on using a library like SFML for graphical development to create a user interface.
6. **Visuals**: Wireframes/Sketches of the interface or the menu driven program
    1. <!-- Hand-drawn sketch in the original PDF, reproduced as text -->

        ```
        +------------------------------------------+
        | Points     Assists    Rebounds           |
        | [1.5x]     [1.0x]     [1.5x]             |
        | Blocks     Steals     Turnovers          |
        | [2.0x]     [1.25x]    [0.75x]            |
        |------------------------------------------|
        | 1. ~~~                                 | |
        | 2. ~~~                                 #  <- Scroll
        | 3. ~~~                                 | |
        | 4. ~~~                                 | |
        | 5. ~~~                                 | |
        +------------------------------------------+
        ```
7. **Strategy**: Preliminary *algorithms* or *data structures* you may want to implement and how would you represent the data
    1. The first data structure we’d likely implement is a max heap built off all the data points that ultimately returns the 25 performances that score highest for the overall score calculated from specific stats.
    2. The second data structure we’d likely implement is a red-black tree built off the same data points that ultimately does a right-to-left traversal and returns the 25 performances that score highest for the overall score calculated from specific stats.
    3. We plan on each performance being stored as an object for which the player’s stats are used with the user weights to calculate an overall score.
8. **Distribution of Responsibility and Roles:** Who is responsible for what?
    1. Jack: Parsing
    2. Thomas: Heap Implementation, Video
    3. Hubert: Red-Black Tree
    4. Team: UI/Design Choices

**References**:

- <https://www.geeksforgeeks.org/dsa/applications-of-heap-data-structure/>
- <https://www.geeksforgeeks.org/dsa/applications-advantages-and-disadvantages-of-red-black-tree/>
- <https://github.com/Ry920/SongSage>
- <https://github.com/Tpascal1/COP3530-Project-2>
- <https://metricgate.com/docs/game-score-basketball/>
