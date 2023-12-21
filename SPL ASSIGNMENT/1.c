#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TEAMS 10
#define MAX_PLAYERS 20
#define MAX_NAME_LENGTH 50

struct Player
{
    char player_name[MAX_NAME_LENGTH];
    int player_number;
    char player_position[4];
    int goals_scored;
    int goals_conceded;
    int number_of_minutes_played;
};

struct Team
{
    char team_name[MAX_NAME_LENGTH];
    int num_players;
    struct Player players[MAX_PLAYERS];
};

struct Team teams[MAX_TEAMS];
int num_teams = 0;

void addPlayerToTeam(char team_name[], struct Player player)
{
    int i;
    for (i = 0; i < num_teams; i++)
    {
        if (strcmp(teams[i].team_name, team_name)==0)
        {
            if (teams[i].num_players < MAX_PLAYERS)
            {
                teams[i].players[teams[i].num_players] = player;
                teams[i].num_players++;
                printf("Player added successfully to team %s.\n", team_name);
            }
            else
            {
                printf("Maximum number of players reached for team %s!\n", team_name);
            }
            return;
        }
    }
    printf("Team not found.\n");
}

void updateTeam(char team_name[], char player_name[], int goals_scored, int goals_conceded)
{
    int i, j;
    for (i = 0; i < num_teams; i++)
    {
        if (strcmp(teams[i].team_name, team_name)==0)
        {
            for (j = 0; j < teams[i].num_players; j++)
            {
                if (strcmp(teams[i].players[j].player_name, player_name)==0)
                {
                    if (strcmp(teams[i].players[j].player_position, "MID")==0 || strcmp(teams[i].players[j].player_position, "FWD")==0)
                    {
                        teams[i].players[j].goals_scored += goals_scored;
                    }
                    if (strcmp(teams[i].players[j].player_position, "DEF")==0)
                    {
                        teams[i].players[j].goals_conceded += goals_conceded;
                    }
                    printf("Team %s updated successfully.\n", team_name);
                    return;
                }
            }
            printf("Player not found in team %s.\n", team_name);
            return;
        }
    }
    printf("Team not found.\n");
}

void displayPlayer(char player_name[])
{
    int i, j;
    for (i = 0; i < num_teams; i++)
    {
        for (j = 0; j < teams[i].num_players; j++)
        {
            if (strcmp(teams[i].players[j].player_name, player_name)==0)
            {
                struct Player player = teams[i].players[j];
                printf("Player Name: %s\n", player.player_name);
                printf("Player Number: %d\n", player.player_number);
                printf("Player Position: %s\n", player.player_position);
                printf("Goals Scored: %d\n", player.goals_scored);
                printf("Goals Conceded: %d\n", player.goals_conceded);
                printf("Number of Minutes Played: %d\n", player.number_of_minutes_played);
                return;
            }
        }
    }
    printf("Player not found.\n");
}

void displayTeam(char team_name[])
{
    int i, j;
    for (i = 0; i < num_teams; i++)
    {
        if (strcmp(teams[i].team_name, team_name)==0)
        {
            struct Team team = teams[i];
            printf("Team Name: %s\n", team.team_name);
            printf("Number of Players: %d\n", team.num_players);
            for (j = 0; j < team.num_players; j++)
            {
                struct Player player = team.players[j];
                printf("Player Name: %s\n", player.player_name);
                printf("Player Number: %d\n", player.player_number);
                printf("Player Position: %s\n", player.player_position);
                printf("Goals Scored: %d\n", player.goals_scored);
                printf("Goals Conceded: %d\n", player.goals_conceded);
                printf("Number of Minutes Played: %d\n", player.number_of_minutes_played);
                printf("----------------\n");
            }
            return;
        }
    }
    printf("Team not found.\n");
}

// Helper function to check if a player already exists in the file
int playerExistsInFile(const char *player_name, FILE *file)
{
    rewind(file); // Move the file position indicator to the beginning of the file

    char name[MAX_NAME_LENGTH];
    while (fscanf(file, "%s", name) != EOF)
    {
        if (strcmp(name, player_name) == 0)
        {
            // Player already exists in the file
            return 1;
        }
    }

    return 0;
}

void saveTeamsToFile()
{
    FILE *file = fopen("football_Database2.txt", "a");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    int i, j;
    for (i = 0; i < num_teams; i++)
    {
        fprintf(file, "\n%s %d\n", teams[i].team_name, teams[i].num_players);
        for (j = 0; j < teams[i].num_players; j++)
        {
            struct Player player = teams[i].players[j];

            // Check if the player data already exists in the file
            if (!playerExistsInFile(player.player_name, file))
            {
                fprintf(file, "%s %d %s %d %d %d\n", player.player_name, player.player_number, player.player_position, player.goals_scored, player.goals_conceded, player.number_of_minutes_played);
            }
        }
    }

    fclose(file);
    printf("Data saved to file.\n");
}

void loadTeamsFromFile()
{
    FILE *file = fopen("football_Database.txt", "r");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return;
    }
    num_teams = 0;
    while (!feof(file) && num_teams < MAX_TEAMS)
    {
        fscanf(file, "%s %d\n", teams[num_teams].team_name, &teams[num_teams].num_players);
        int i;
        for (i = 0; i < teams[num_teams].num_players; i++)
        {
            struct Player player;
            fscanf(file, "%s %d %s %d %d %d\n", player.player_name, &player.player_number, player.player_position, &player.goals_scored, &player.goals_conceded, &player.number_of_minutes_played);
            teams[num_teams].players[i] = player;
        }
        num_teams++;
    }
    fclose(file);
    printf("Data loaded from file.\n");
}

void saveStatsToFile()
{
    FILE *file = fopen("stats.txt", "w");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    int i, j;
    int max_goals_scored = 0;
    int min_goals_conceded = 999;
    struct Player max_goals_player;
    struct Player min_goals_player;
    struct Player max_minutes_player;

    for (i = 0; i < num_teams; i++)
    {
        for (j = 0; j < teams[i].num_players; j++)
        {
            struct Player player = teams[i].players[j];
            if (player.goals_scored > max_goals_scored)
            {
                max_goals_scored = player.goals_scored;
                max_goals_player = player;
            }
            if (player.goals_conceded < min_goals_conceded)
            {
                min_goals_conceded = player.goals_conceded;
                min_goals_player = player;
            }
            if (player.number_of_minutes_played > max_minutes_player.number_of_minutes_played)
            {
                max_minutes_player = player;
            }
        }
    }

    fprintf(file, "Most goals scored: %s %d\n", max_goals_player.player_name, max_goals_player.goals_scored);
    fprintf(file, "Least goals conceded: %s %d\n", min_goals_player.player_name, min_goals_player.goals_conceded);
    fprintf(file, "Most minutes played: %s %d %d\n", max_minutes_player.player_name, max_minutes_player.player_number, max_minutes_player.number_of_minutes_played); // Change %s to %d

    fclose(file);
    printf("Stats saved to file.\n");
}

// Helper function to check if a team already exists
int teamExists(const char *team_name)
{
    for (int i = 0; i < num_teams; i++)
    {
        if (strcmp(teams[i].team_name, team_name) == 0)
        {
            // Team already exists
            return 1;
        }
    }

    // Team does not exist
    return 0;
}

void createNewTeam()
{
    if (num_teams < MAX_TEAMS)
    {
        printf("Enter the name of the new team: ");
        char new_team_name[MAX_NAME_LENGTH];
        scanf("%s", new_team_name);

        // Check if the team already exists
        if (teamExists(new_team_name))
        {
            printf("Team '%s' already exists.\n", new_team_name);
            return;
        }

        // Set the name of the new team
        strcpy(teams[num_teams].team_name, new_team_name);

        // Initialize the number of players for the new team to 0
        teams[num_teams].num_players = 0;

        printf("Team '%s' created successfully.\n", new_team_name);

        // Increment the total number of teams
        num_teams++;
    }
    else
    {
        printf("Maximum number of teams reached.\n");
    }
}

void displayAllTeams()
{
    if (num_teams > 0)
    {
        printf("Football Database - All Teams\n");
        for (int i = 0; i < num_teams; i++)
        {
            printf("Team Name: %s\n", teams[i].team_name);
            printf("Number of Players: %d\n", teams[i].num_players);

            if (teams[i].num_players > 0)
            {
                printf("Players:\n");
                for (int j = 0; j < teams[i].num_players; j++)
                {
                    struct Player player = teams[i].players[j];
                    printf("  Player Name: %s\n", player.player_name);
                    printf("  Player Number: %d\n", player.player_number);
                    printf("  Player Position: %s\n", player.player_position);
                    printf("  Goals Scored: %d\n", player.goals_scored);
                    printf("  Goals Conceded: %d\n", player.goals_conceded);
                    printf("  Number of Minutes Played: %d\n", player.number_of_minutes_played);
                    printf("  ----------------\n");
                }
            }
            else
            {
                printf("No players in this team.\n");
            }

            printf("----------------\n");
        }
    }
    else
    {
        printf("No teams in the football database.\n");
    }
}

int main()
{
    int choice;
    char team_name[MAX_NAME_LENGTH];
    char player_name[MAX_NAME_LENGTH];
    int goals_scored, goals_conceded;
    struct Player player;

    while (1)
    {
        printf("\nFootball Database Management System\n");
        printf("1. Add player to team\n");
        printf("2. Update team\n");
        printf("3. Display player\n");
        printf("4. Display team\n");
        printf("5. Save teams to file\n");
        printf("6. Load teams from file\n");
        printf("7. Save stats to file\n");
        printf("8. Create new Team\n");
        printf("9. Display all team info\n");
        printf("10. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Enter team name: ");
            scanf("%s", team_name);
            printf("Enter player name: ");
            scanf("%s", player.player_name);
            printf("Enter player number: ");
            scanf("%d", &player.player_number);
            printf("Enter player position (MID, FWD, or DEF): ");
            scanf("%s", player.player_position);
            printf("Enter goals scored: ");
            scanf("%d", &player.goals_scored);
            printf("Enter goals conceded: ");
            scanf("%d", &player.goals_conceded);
            printf("Enter number of minutes played: ");
            scanf("%d", &player.number_of_minutes_played);
            addPlayerToTeam(team_name, player);
            break;
        case 2:
            printf("Enter team name: ");
            scanf("%s", team_name);
            printf("Enter player name: ");
            scanf("%s", player_name);
            printf("Enter goals scored: ");
            scanf("%d", &goals_scored);
            printf("Enter goals conceded: ");
            scanf("%d", &goals_conceded);
            updateTeam(team_name, player_name, goals_scored, goals_conceded);
            break;
        case 3:
            printf("Enter player name: ");
            scanf("%s", player_name);
            displayPlayer(player_name);
            break;
        case 4:
            printf("Enter team name: ");
            scanf("%s", team_name);
            displayTeam(team_name);
            break;
        case 5:
            saveTeamsToFile();
            break;
        case 6:
            loadTeamsFromFile();
            break;
        case 7:
            saveStatsToFile();
            break;
        case 8:
            createNewTeam();
            break;

        case 9:
            displayAllTeams();
            break;
        case 10:
            printf("Exiting... Thank you!\n");
            exit(0);
        default:
            printf("Invalid choice. Please try again.\n");
            break;
        }
    }

    return 0;
}