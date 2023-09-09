#include "lanparty.h"

match_queue_t *create_queue(team_node_t *team_list_head)
{
    // Allocate memory for the match queue structure.
    match_queue_t *match_queue = malloc(sizeof(match_queue_t));
    DIE(match_queue == NULL, "Error allocating memory for match queue structure.");

    // Initialize the front, rear, and head pointers of the match queue to NULL.
    match_queue->front = match_queue->rear = match_queue->head = NULL;

    // Iterate over the team list to enqueue matches between teams.
    while (team_list_head != NULL)
    {
        // Enqueue a match between the current team and its next team in the list.
        enqueue_match(match_queue, team_list_head, team_list_head->next);

        // Move the team_list_head pointer to skip the next team in the list,
        // as it was already enqueued.
        team_list_head = team_list_head->next->next;
    }

    // Return the pointer to the created match queue.
    return match_queue;
}

void enqueue_match(match_queue_t *match_queue, team_node_t *first_team,
                   team_node_t *second_team)
{
    // Create a new match node and allocate memory for it.
    match_node_t *new_match_node = malloc(sizeof(match_node_t));
    DIE(new_match_node == NULL, "Error allocating memory for new_match_node.");

    // Assign the first and second teams to the new match node.
    new_match_node->first_team = first_team;
    new_match_node->second_team = second_team;

    // Set the next pointer of the new match node to NULL.
    new_match_node->next = NULL;

    // If the match queue is empty, make the new match node the rear of the queue.
    if (match_queue->rear == NULL)
    {
        match_queue->rear = new_match_node;
    }
    else
    {
        // Otherwise, append the new match node to the rear of the queue.
        match_queue->rear->next = new_match_node;
        match_queue->rear = new_match_node;
    }

    // If the match queue was initially empty,
    // set the front and head pointers to the new match node.
    if (match_queue->front == NULL)
    {
        match_queue->front = match_queue->rear;
        match_queue->head = match_queue->front;
    }
}

match_node_t *dequeue_match(match_queue_t *match_queue)
{
    // If the front pointer is NULL, the queue is empty, so return NULL.
    if (match_queue->front == NULL)
        return NULL;

    // Store the current front match node in a temporary variable to return it later.
    match_node_t *match_node = match_queue->front;

    // Update the front pointer to the next match node in the queue.
    match_queue->front = match_queue->front->next;

    // Return the dequeued match node.
    return match_node;
}

void dealloc_match_queue_between_rounds(match_queue_t *match_queue)
{
    match_node_t *temp = NULL;

    // Iterate over the match queue and free each match node, as it wasn't freed when dequeued.
    while (match_queue->head != NULL)
    {
        temp = match_queue->head;
        match_queue->head = match_queue->head->next;
        free(temp);
    }

    // Reset the front, rear, and head pointers of the match queue to NULL.
    match_queue->front = NULL;
    match_queue->rear = NULL;
    match_queue->head = NULL;
}

void stack_push(stack_node_t **stack_top, team_node_t *value)
{
    // Create a new stack node and allocate memory for it.
    stack_node_t *new_stack_node = malloc(sizeof(stack_node_t));
    DIE(new_stack_node == NULL, "Error allocating memory for new_stack_node.");

    // Assign the value (pointer to team node) to the new stack node.
    new_stack_node->value = value;

    // Set the next pointer of the new stack node to the current stack top.
    new_stack_node->next = (*stack_top);

    // Update the stack top to point to the new stack node.
    *stack_top = new_stack_node;
}

team_node_t *stack_pop(stack_node_t **stack_top)
{
    // If the stack is empty (stack top is NULL), return NULL.
    if (*stack_top == NULL)
        return NULL;

    // Store the current stack top in a temporary variable.
    stack_node_t *temp = (*stack_top);

    // Retrieve the value (team node) from the stack top.
    team_node_t *value = (*stack_top)->value;

    // Update the stack top to point to the next node in the stack.
    *stack_top = (*stack_top)->next;

    // Free the memory allocated for the popped stack node.
    free(temp);

    // Return the popped team node.
    return value;
}

int get_teams_count(team_node_t *team_list_head)
{
    int no_of_teams = 0;

    // Iterate over the team list and increment the count for each team.
    while (team_list_head != NULL)
    {
        no_of_teams++;
        team_list_head = team_list_head->next;
    }

    // Return the total count of teams.
    return no_of_teams;
}

tree_node_t *new_tree_node(team_node_t *data)
{
    // Create a new tree node and allocate memory for it.
    tree_node_t *tree_node = malloc(sizeof(tree_node_t));
    DIE(tree_node == NULL, "Error allocating memory for tree_node.");

    // Assign the data (pointer to team node) to the new tree node.
    tree_node->value = data;

    // Set the left and right child pointers of the tree node to NULL.
    tree_node->left = tree_node->right = NULL;

    // Set the height of the tree node to 1.
    tree_node->height = 1;

    // Return the new tree node.
    return tree_node;
}

int node_height(tree_node_t *tree_node)
{
    // If the tree_node is NULL, return 0.
    if (tree_node == NULL)
        return 0;
    else
        // Otherwise, return the height of the tree_node.
        return tree_node->height;
}

int max(int a, int b)
{
    // Return the larger value between a and b using a ternary operator.
    return (a > b) ? a : b;
}

void delete_team_node(team_node_t *node, team_node_t **head)
{
    team_node_t *current_node = NULL;

    // If the node to be deleted is the head node, update the head pointer.
    if (node == (*head))
    {
        *head = node->next;
    }
    else
    {
        // Otherwise, find the previous node of the node to be deleted.
        current_node = (*head);
        while (current_node->next != node)
        {
            current_node = current_node->next;
        }

        // Update the next pointer of the previous node to skip the node to be deleted.
        current_node->next = node->next;
    }

    // Deallocate the memory used by the player list of the team node.
    dealloc_player_list(node->head);

    // Free the memory allocated for the team name string.
    free(node->team_name);

    // Free the memory allocated for the team node.
    free(node);
}

void dealloc_player_list(player_node_t *head)
{
    player_node_t *temp = NULL;

    // Iterate over the player list and free each player node.
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp->player.firstName);
        free(temp->player.secondName);
        free(temp);
    }
}

void dealloc_team_list(team_node_t *head)
{
    team_node_t *temp = NULL;

    // Iterate over the team list and free each team node.
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        dealloc_player_list(temp->head);
        free(temp->team_name);
        free(temp);
    }
}

void dealloc_tree(tree_node_t *root)
{
    if (root == NULL)
        return;

    // Recursively deallocate the left and right subtrees.
    dealloc_tree(root->left);
    dealloc_tree(root->right);

    // Free the memory allocated for the current tree node.
    free(root);
}

void deallocate_memory(team_node_t *team_list_head, team_node_t *rank8_list_head,
                       tree_node_t *bst_root, tree_node_t *avl_root, FILE *teams_input,
                       FILE *requests_input, FILE *output_file)
{
    // Deallocate the memory used by the team list.
    if (team_list_head != NULL)
        dealloc_team_list(team_list_head);

    // Deallocate the memory used by the rank8 team list.
    if (rank8_list_head != NULL)
        dealloc_team_list(rank8_list_head);

    // Deallocate the memory used by the BST.
    if (bst_root != NULL)
        dealloc_tree(bst_root);

    // Deallocate the memory used by the AVL tree.
    if (avl_root != NULL)
        dealloc_tree(avl_root);

    // Close the teams input file.
    fclose(teams_input);

    // Close the requests input file.
    fclose(requests_input);

    // Close the output file.
    fclose(output_file);
}