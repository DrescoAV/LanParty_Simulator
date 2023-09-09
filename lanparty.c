#include "lanparty.h"

team_node_t *create_team_list(FILE *teams_input)
{
    int no_of_teams, no_of_players, i, j;
    char input_buffer, aux[256];

    // Read the number of teams from the file.
    fscanf(teams_input, "%d%c", &no_of_teams, &input_buffer);

    // Initialize the head and new_node_team pointers to NULL.
    team_node_t *team_list_head = NULL;
    team_node_t *new_node_team = NULL;

    // Loop to create each team.
    for (i = 1; i <= no_of_teams; i++)
    {
        // Allocate memory for a new team_node_t structure.
        new_node_team = malloc(sizeof(team_node_t));
        DIE(new_node_team == NULL, "Error allocating memory for new_node_team.");

        // Initialize the members of the new node.
        new_node_team->head = NULL;
        new_node_team->team_name = NULL;
        new_node_team->next = NULL;
        new_node_team->team_score = 0;

        // Read the number of players for the current team.
        fscanf(teams_input, "%d%c", &no_of_players, &input_buffer);

        // Read the team name and store it in the new_node_team.
        fgets(aux, 256, teams_input);
        aux[strlen(aux) - 2] = '\0';
        if (aux[strlen(aux) - 1] == ' ')
            aux[strlen(aux) - 1] = '\0';
        new_node_team->team_name = malloc(strlen(aux) + 1);
        DIE(new_node_team->team_name == NULL,
            "Error allocating memory for new_node_team->team_name.");
        strcpy(new_node_team->team_name, aux);

        // Loop to add players to the current team.
        for (j = 1; j <= no_of_players; j++)
        {
            add_player_at_beggining(teams_input, &new_node_team->head,
                                    &new_node_team->team_score);
        }

        // Calculate the average team score.
        new_node_team->team_score /= no_of_players;

        // Read the input buffer character to handle newline.
        fscanf(teams_input, "%c", &input_buffer);

        // Set the next pointer of new_node_team to the current team_list_head.
        new_node_team->next = team_list_head;

        // Update the team_list_head to the new_node_team.
        team_list_head = new_node_team;
    }

    // Return the head of the team list.
    return team_list_head;
}

void add_player_at_beggining(FILE *teams_input, player_node_t **head, double *team_score)
{
    player_node_t *new_node = malloc(sizeof(player_node_t));
    DIE(new_node == NULL, "Error allocating memory for new_node.");

    char input_buffer[256];

    // Read the first name of the player from the file.
    fscanf(teams_input, "%s", input_buffer);

    // Allocate memory and store the first name in the new_node.
    new_node->player.firstName = malloc(strlen(input_buffer) + 1);
    DIE(new_node->player.firstName == NULL,
        "Error allocating memory for new_node->player.firstName.");
    strcpy(new_node->player.firstName, input_buffer);

    // Read in the same buffer the second name of the player and the points from the file.
    fgets(input_buffer, sizeof(input_buffer), teams_input);

    // Remove '\n' and '\r' from the string.
    input_buffer[strlen(input_buffer) - 2] = '\0';

    // Create a buffer to store the points as a string.
    char *points_string = malloc(strlen(strrchr(input_buffer, ' ')));
    DIE(points_string == NULL, "Error allocating memory for points_string.");
    memmove(points_string, strrchr(input_buffer, ' ') + 1, strlen(strrchr(input_buffer, ' ')));

    // Transform the player's points from string to integer using atoi().
    new_node->player.points = atoi(points_string);

    // Remove the points from the second name string.
    input_buffer[strlen(input_buffer) - strlen(points_string) - 2] = '\0';

    // Allocate memory and store the second name in the new_node.
    new_node->player.secondName = malloc(strlen(input_buffer) + 1);
    DIE(new_node->player.secondName == NULL,
        "Error allocating memory for new_node->player.secondName.");
    strcpy(new_node->player.secondName, input_buffer);

    // Free alocated memory for the points buffer.
    free(points_string);

    // Update the team score by adding the points of the player.
    *team_score = (*team_score) + new_node->player.points;

    // Set the next pointer of new_node to the current head.
    new_node->next = (*head);

    // Update the head to the new_node.
    *head = new_node;
}

int check_2_power(int number)
{
    while (number % 2 == 0)
        number /= 2;

    // Check if the number is a power of 2.
    if (number == 1)
        return 1;
    return 0;
}

void delete_lowest_points_teams(team_node_t **team_list_head)
{
    int no_of_teams = get_teams_count(*team_list_head);
    team_node_t *current_node = NULL;
    double min_score = -1;

    // Delete teams until the number of teams becomes a power of 2.
    while (check_2_power(no_of_teams) == 0)
    {
        // Find the minimum team score.
        min_score = (*team_list_head)->team_score;
        current_node = (*team_list_head);
        while (current_node != NULL)
        {
            if (current_node->team_score < min_score)
            {
                min_score = current_node->team_score;
            }
            current_node = current_node->next;
        }

        // Find the first team with the minimum score.
        current_node = (*team_list_head);
        while (current_node != NULL && current_node->team_score != min_score)
        {
            current_node = current_node->next;
        }

        // Delete the team node if found.
        if (current_node != NULL)
            delete_team_node(current_node, team_list_head);

        // Update the number of teams.
        no_of_teams--;
    }
}

void delete_lowest_points_teams_using_array(team_node_t **team_list_head)
{
    int no_of_teams = get_teams_count(*team_list_head);
    team_node_t *current_node = (*team_list_head);
    double *team_scores_array = malloc(sizeof(double) * no_of_teams);
    DIE(team_scores_array == NULL,
        "Error allocating memory for team_scores_array.");
    int i = 0, j;

    // Store the team scores in an array.
    while (current_node != NULL)
    {
        team_scores_array[i++] = current_node->team_score;
        current_node = current_node->next;
    }

    int no_of_eliminations = 0;

    // Get the number of eliminations required to make the number of teams a power of 2.
    while (check_2_power(no_of_teams) == 0)
    {
        no_of_teams--;
        no_of_eliminations++;
    }

    no_of_teams += no_of_eliminations;
    double aux;

    // Sort the team scores in ascending order.
    for (i = 0; i < no_of_eliminations; i++)
    {
        for (j = i + 1; j < no_of_teams; j++)
        {
            if (team_scores_array[j] < team_scores_array[i])
            {
                aux = team_scores_array[i];
                team_scores_array[i] = team_scores_array[j];
                team_scores_array[j] = aux;
            }
        }
    }

    // Delete teams with the lowest scores based on the sorted array.
    for (i = 0; i < no_of_eliminations; i++)
    {
        current_node = (*team_list_head);
        while (current_node != NULL && current_node->team_score != team_scores_array[i])
            current_node = current_node->next;
        delete_team_node(current_node, team_list_head);
    }

    free(team_scores_array);
}

void display_request_1_2(FILE *output_file, team_node_t *team_list_head)
{
    while (team_list_head != NULL)
    {
        // Write the team name to the output file.
        fprintf(output_file, "%s\n", team_list_head->team_name);

        // Update the current node with the next one.
        team_list_head = team_list_head->next;
    }
}

void copy_players_list(team_node_t *team_node, team_node_t **new_team_node)
{
    player_node_t *current_player_node = team_node->head;
    player_node_t *new_player_node = NULL;
    player_node_t *player_list_head = NULL;

    // Copy the player list from the original team to the new team
    while (current_player_node != NULL)
    {
        // Allocate memory for the new player node.
        new_player_node = malloc(sizeof(player_node_t));
        DIE(new_player_node == NULL, "Error allocating memory for new_player_node.");

        // Allocate memory for the player's names and copy them into the created node.
        new_player_node->player.firstName = malloc(strlen(current_player_node->player.firstName) + 1);
        DIE(new_player_node->player.firstName == NULL,
            "Error allocating memory for new_player_node->player.firstName.");
        strcpy(new_player_node->player.firstName,
               current_player_node->player.firstName);
        new_player_node->player.secondName = malloc(strlen(current_player_node->player.secondName) + 1);
        DIE(new_player_node->player.secondName == NULL,
            "Error allocating memory for new_player_node->player.secondName.");
        strcpy(new_player_node->player.secondName,
               current_player_node->player.secondName);

        // Copy the player's score and initialize the next pointer with NULL.
        new_player_node->player.points = current_player_node->player.points;
        new_player_node->next = NULL;

        // If the head of the player list is NULL, append the new player node to it.
        if ((*new_team_node)->head == NULL)
            (*new_team_node)->head = new_player_node;
        else
        {
            // Otherwise, traverse to the end of the list.
            player_list_head = (*new_team_node)->head;
            while (player_list_head->next != NULL)
                player_list_head = player_list_head->next;

            // Append the new player node to the end of the player list.
            player_list_head->next = new_player_node;
        }

        // Update the current player node from the team with the next one.
        current_player_node = current_player_node->next;
    }
}

team_node_t *create_ranking8(team_node_t *team_list_head)
{
    team_node_t *new_team_node = NULL, *temp_team_node = NULL, *rank8_list_head = NULL;
    team_node_t *current_team_node = team_list_head;
    int i;

    // Create a ranking list of the top 8 teams.
    for (i = 1; i <= 8; i++)
    {
        // Allocate memory for a new team node and initialize it.
        new_team_node = malloc(sizeof(team_node_t));
        DIE(new_team_node == NULL, "Error allocating memory for new_team_node.");
        new_team_node->head = NULL;
        new_team_node->next = NULL;

        // If the head is NULL, the new node becomes it.
        if (rank8_list_head == NULL)
            rank8_list_head = new_team_node;

        // Copy the players list from the current team to the new team.
        copy_players_list(current_team_node, &new_team_node);

        // Allocate memory for the team name and copy it into the created node.
        new_team_node->team_name = malloc(strlen(current_team_node->team_name) + 1);
        DIE(new_team_node->team_name == NULL,
            "Error allocating memory for new_team_node->team_name.");
        strcpy(new_team_node->team_name, current_team_node->team_name);
        new_team_node->team_score = current_team_node->team_score;

        // Append the new team node to end of the ranking list.
        if (i > 1)
        {
            temp_team_node = rank8_list_head;
            while (temp_team_node->next != NULL)
                temp_team_node = temp_team_node->next;
            temp_team_node->next = new_team_node;
        }

        // Update the current team node with the next one.
        current_team_node = current_team_node->next;
    }

    return rank8_list_head;
}

team_node_t *gaming_phase(team_node_t **team_list_head, FILE *output_file)
{
    team_node_t *rank8_list_head = NULL;
    stack_node_t *winners_stack_top = NULL, *losers_stack_top = NULL;
    int round_no = 1;

    // Getting number of teams.
    int no_of_teams = get_teams_count(*team_list_head);

    // Create a match queue for the gaming phase.
    match_queue_t *match_queue = create_queue(*team_list_head);

    // Perform the gaming phase until there is a single winner.
    while (no_of_teams != 1)
    {
        // Create a ranking list of the top 8 teams.
        if (no_of_teams == 8)
        {
            rank8_list_head = create_ranking8(*team_list_head);
        }

        // Perform ongoing matches, update winners & losers stacks and print the results.
        ongoing_round(match_queue, &winners_stack_top, &losers_stack_top,
                      output_file, round_no);

        // Deallocate the match queue between rounds.
        dealloc_match_queue_between_rounds(match_queue);

        // Enqueue the winners to the match queue and print them.
        enqueue_and_print_winners(output_file, &winners_stack_top, match_queue,
                                  round_no);

        // Delete the loser teams from the team list.
        delete_loser_teams(&losers_stack_top, team_list_head);

        // Increase round number and decrease the number of teams.
        round_no++;
        no_of_teams /= 2;
    }

    // Deallocate the match queue.
    free(match_queue);

    // Return the head of the top 8 teams list.
    return rank8_list_head;
}

void ongoing_round(match_queue_t *match_queue, stack_node_t **winners_stack_top,
                   stack_node_t **losers_stack_top, FILE *output_file, int round_no)
{
    team_node_t *loser_team = NULL, *winner_team = NULL;
    player_node_t *player_node = NULL;
    match_node_t *match_node = NULL;

    // Print round number.
    fprintf(output_file, "\n--- ROUND NO:%d\n", round_no);

    // Perform ongoing matches until the match queue is empty.
    do
    {
        // Dequeue match from match queue.
        match_node = dequeue_match(match_queue);

        // Check if the match node exists.
        if (match_node != NULL)
        {
            // Determine the winner and loser teams based on their scores.
            if (match_node->first_team->team_score > match_node->second_team->team_score)
            {
                winner_team = match_node->first_team;
                loser_team = match_node->second_team;
            }
            else
            {
                winner_team = match_node->second_team;
                loser_team = match_node->first_team;
            }

            // Print the teams.
            fprintf(output_file, "%-32s - %+32s\n", match_node->first_team->team_name,
                    match_node->second_team->team_name);

            // Increase the team score of the winner and update player points.
            winner_team->team_score++;
            player_node = winner_team->head;
            while (player_node != NULL)
            {
                player_node->player.points++;
                player_node = player_node->next;
            }

            // Push the winner and loser teams to the respective stacks.
            stack_push(winners_stack_top, winner_team);
            stack_push(losers_stack_top, loser_team);
        }
    } while (match_node != NULL);
}

void enqueue_and_print_winners(FILE *output_file, stack_node_t **winners_stack_top,
                               match_queue_t *match_queue, int round_no)
{
    team_node_t *first_team = NULL, *second_team = NULL;

    // Print round number.
    fprintf(output_file, "\nWINNERS OF ROUND NO:%d\n", round_no);

    // Enqueue the winners from the stack to the match queue and print them.
    do
    {
        first_team = stack_pop(winners_stack_top);
        second_team = stack_pop(winners_stack_top);

        if (first_team != NULL)
            fprintf(output_file, "%-33s -  %.2f\n", first_team->team_name,
                    first_team->team_score);

        if (second_team != NULL)
            fprintf(output_file, "%-33s -  %.2f\n", second_team->team_name,
                    second_team->team_score);

        if (first_team != NULL && second_team != NULL)
            enqueue_match(match_queue, first_team, second_team);

    } while (first_team != NULL);
}

void delete_loser_teams(stack_node_t **losers_stack_top, team_node_t **team_list_head)
{
    team_node_t *first_team = NULL, *second_team = NULL;

    // Delete the loser teams from the team list.
    do
    {
        first_team = stack_pop(losers_stack_top);
        second_team = stack_pop(losers_stack_top);

        if (first_team != NULL)
            delete_team_node(first_team, team_list_head);

        if (second_team != NULL)
            delete_team_node(second_team, team_list_head);

    } while (first_team != NULL);
}

void create_bst(team_node_t *rank8_list_head, tree_node_t **bst_root)
{
    // Traverse through the rank8 list and insert each team into the binary search tree.
    while (rank8_list_head != NULL)
    {
        // Insert the current team node into the binary search tree.
        *bst_root = bst_insert(*bst_root, rank8_list_head);

        // Update the current team node with the next one.
        rank8_list_head = rank8_list_head->next;
    }
}

tree_node_t *bst_insert(tree_node_t *tree_node, team_node_t *team_node)
{
    // If the current tree node is NULL, create a new tree node with the given team node.
    if (tree_node == NULL)
        return new_tree_node(team_node);

    // Compare the team scores of the team node and the tree node's value.
    // If the score values are equal, it will compare their names.
    if (team_node->team_score == tree_node->value->team_score)
    {
        // It will decide the insertion based on lexicographical order.
        // The greater one will be the first.
        if (strcmp(team_node->team_name, tree_node->value->team_name) > 0)
            tree_node->left = bst_insert(tree_node->left, team_node);
        else
            tree_node->right = bst_insert(tree_node->right, team_node);
    }
    // If the scores are diffrent, it will insert them in descending order.
    else if (team_node->team_score < tree_node->value->team_score)
        tree_node->right = bst_insert(tree_node->right, team_node);
    else if (team_node->team_score > tree_node->value->team_score)
        tree_node->left = bst_insert(tree_node->left, team_node);

    return tree_node;
}

void in_order_traversal(tree_node_t *tree_root, FILE *output_file)
{
    // Perform an in-order traversal of the BST and print the team names and scores.
    if (tree_root == NULL)
        return;

    in_order_traversal(tree_root->left, output_file);
    fprintf(output_file, "%-33s -  %.2f\n", tree_root->value->team_name,
            tree_root->value->team_score);
    in_order_traversal(tree_root->right, output_file);
}

tree_node_t *right_rotation(tree_node_t *y)
{
    tree_node_t *x = y->left;
    tree_node_t *T2 = x->right;

    // Perform the rotation.
    x->right = y;
    y->left = T2;

    // Update heights.
    y->height = max(node_height(y->left), node_height(y->right)) + 1;
    x->height = max(node_height(x->left), node_height(x->right)) + 1;

    // Return the new root of the rotated subtree.
    return x;
}

tree_node_t *left_rotation(tree_node_t *x)
{
    tree_node_t *y = x->right;
    tree_node_t *T2 = y->left;

    // Perform the rotation.
    y->left = x;
    x->right = T2;

    // Update heights.
    x->height = max(node_height(x->left), node_height(x->right)) + 1;
    y->height = max(node_height(y->left), node_height(y->right)) + 1;

    // Return the new root of the rotated subtree.
    return y;
}

tree_node_t *right_left_rotation(tree_node_t *z)
{
    z->right = right_rotation(z->right);
    return left_rotation(z);
}

tree_node_t *left_right_rotation(tree_node_t *z)
{
    z->left = left_rotation(z->left);
    return right_rotation(z);
}

int get_balance(tree_node_t *tree_node)
{
    if (tree_node == NULL)
        return 0;
    return node_height(tree_node->left) - node_height(tree_node->right);
}

// Extract team nodes from the AVL tree using an in-order traversal and modify their scores.
void in_order_extraction(tree_node_t *root, team_node_t **arr)
{
    static int i = 0;
    int j;
    // Rule to exit recursion.
    if (root == NULL)
        return;
    in_order_extraction(root->left, arr);

    // Copy the pointer to the team node from the BST tree into the array.
    arr[i++] = root->value;

    // Increase scores with an insignificant number so there are no equal scores.
    // They will be in descending order.
    for (j = 0; j < i; j++)
    {
        arr[j]->team_score += 0.0001;
    }
    in_order_extraction(root->right, arr);
}

void create_avl(tree_node_t **avl_root, tree_node_t *bst_root)
{
    // Allocate memory for an array of pointers to temporary store the rank8 list.
    team_node_t **arr = malloc(sizeof(team_node_t *) * RANK8_SIZE);
    DIE(arr == NULL, "Error allocating memory for array of pointers.");

    // Extract team nodes from BST in an in-order traversal.
    // Modify their scores if they have the same one.
    in_order_extraction(bst_root, arr);

    // Insert team nodes into the AVL tree.
    int i;
    for (i = 0; i < 8; i++)
    {
        *avl_root = avl_insert(*avl_root, arr[i]);
    }

    // Free allocated memory for the array.
    free(arr);
}

tree_node_t *avl_insert(tree_node_t *tree_node, team_node_t *team_node)
{
    // If the current tree node is NULL, create a new tree node with the given team node.
    if (tree_node == NULL)
        return (new_tree_node(team_node));

    // Insert the tree node in descending order.
    if (team_node->team_score > tree_node->value->team_score)
        tree_node->left = avl_insert(tree_node->left, team_node);
    else if (team_node->team_score < tree_node->value->team_score)
        tree_node->right = avl_insert(tree_node->right, team_node);
    else
        // No duplicates are allowed.
        return tree_node;

    // Update the height of the tree node.
    tree_node->height = 1 + max(node_height(tree_node->left), node_height(tree_node->right));

    // Get the balance factor of the tree node.
    int balance = get_balance(tree_node);

    // Perform appropriate rotations based on the balance factor.
    if (balance > 1 && team_node->team_score > tree_node->left->value->team_score)
        return right_rotation(tree_node);
    if (balance < -1 && team_node->team_score < tree_node->right->value->team_score)
        return left_rotation(tree_node);
    if (balance > 1 && team_node->team_score < tree_node->left->value->team_score)
        return left_right_rotation(tree_node);
    if (balance < -1 && team_node->team_score > tree_node->right->value->team_score)
        return right_left_rotation(tree_node);

    return tree_node;
}

void current_level_order_traversal(FILE *output_file, tree_node_t *root, int level)
{
    // Rule to exit recurison.
    if (root == NULL)
    {
        return;
    }
    // For the selected level, it will print the team's name.
    if (level == 0)
    {
        fprintf(output_file, "%s\n", root->value->team_name);
    }
    else if (level > 0)
    {
        // Go to the next level using recursion.
        current_level_order_traversal(output_file, root->left, level - 1);
        current_level_order_traversal(output_file, root->right, level - 1);
    }
}
