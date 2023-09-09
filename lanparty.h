#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define SECOND_LEVEL 2
#define RANK8_SIZE 8

// Macro for managing errors during memory allocation.
#define DIE(assertion, call_description)                       \
    do                                                         \
    {                                                          \
        if (assertion)                                         \
        {                                                      \
            fprintf(stderr, "(%s, %d): ", __FILE__, __LINE__); \
            perror(call_description);                          \
            exit(errno);                                       \
        }                                                      \
    } while (0)

// Player strcture.
typedef struct player_t
{
    char *firstName;
    char *secondName;
    int points;
} player_t;

// Player node strucure.
typedef struct player_node_t
{
    player_t player;
    struct player_node_t *next;
} player_node_t;

// Team node structure.
typedef struct team_node_t
{
    player_node_t *head;
    char *team_name;
    double team_score;
    struct team_node_t *next;
} team_node_t;

// Match node structure.
typedef struct match_node_t
{
    team_node_t *first_team, *second_team;
    struct match_node_t *next;
} match_node_t;

// Stack node structure.
typedef struct stack_node_t
{
    team_node_t *value;
    struct stack_node_t *next;
} stack_node_t;

// Match queue structure.
typedef struct match_queue_t
{
    match_node_t *front, *rear, *head;
} match_queue_t;

// Tree node structure.
typedef struct tree_node_t
{
    team_node_t *value;
    int height;
    struct tree_node_t *right, *left;
} tree_node_t;

/*
 * create_team_list() - Is responsible for creating the list of teams.
 * @arg1: The input file.
 * It returns a pointer to the head of the created list.
 */
team_node_t *create_team_list(FILE *teams_input);

/*
 * add_player_at_beggining() - For each teams it adds a player at the
                               beggining of the player list.
 * @arg1: The input file.
 * @arg2: Pointer to player's list head's address, to modify it if nedeed.
 * @arg3: Pointer to team score's address, to modify it if nedeed.
 */
void add_player_at_beggining(FILE *teams_input, player_node_t **head, double *team_score);

/*
 * check_2_power() - Check if the provided number is a power of 2.
 * @arg1: The number to check.
 * Returns 1 if the number is a power of 2, or 0 otherwise.
 */
int check_2_power(int number);

/*
 * get_teams_count() - Get the number of teams in the team list.
 * @arg1: The team list head.
 * Returns the number of teams in the team list.
 */
int get_teams_count(team_node_t *team_list_head);

/*
 * delete_team_node() - Deletes given team from the team list.
 * @arg1: The node to be deleted from the list.
 * @arg2: Pointer to the team's head address, so it can modify it, if nedded to delete it.
 */
void delete_team_node(team_node_t *node, team_node_t **head);

/*
 * delete_lowest_points_teams_ussing_array() - Delete teams with the lowest points until
                                               the number of teams is a power of 2.
 * @arg1: Pointer to the team's head address, so it can modify it, if nedded to delete it.
 * This function uses an array to store the scores for each team, and sorts the first n
 * scores needed to be removed. This way, there is no need to find the minimum score after
 * each elimination.
 */
void delete_lowest_points_teams_using_array(team_node_t **team_list_head);

/*
 * delete_lowest_points_teams() - Delete teams with the lowest points until
                                  the number of teams is a power of 2.
 * @arg1: Pointer to the team's head address, so it can modify it, if nedded to delete it.
 * Unlike delete_lowest_points_teams_ussing_array, this function needs to find the minimum
 * score after each team removal and remove the next one.
 */
void delete_lowest_points_teams(team_node_t **team_list_head);

/*
 * display_request_1_2() - Prints to output file the team names.
 * @arg1: Output file to print into.
 * @arg2: Team list head to iterate over the list and print the names.
 */
void display_request_1_2(FILE *output_file, team_node_t *team_list_head);

/*
 * create_queue() - Allocates memory for the match queue and initialises the head, front
                    and rear pointer with NULL.
 * @arg1: The team list head, used to enqueue all the teams in the created match queue.
 * Returns pointer to the created match queue.
 */
match_queue_t *create_queue(team_node_t *team_list_head);

/*
 * enqueue_match() - Function used to place inside the queue of matches a match formed of
                     2 teams.
 * @arg1: The queue of matches.
 * @arg2: First team to be added in the match.
 * @arg3: Second team to be added in the match.
 */
void enqueue_match(match_queue_t *match_queue, team_node_t *first_team,
                   team_node_t *second_team);

/*
 * dequeue_match() - Removes the front match from the queue.
 * @arg1: Pointer to the match queue.
 * Returns the match.
 */
match_node_t *dequeue_match(match_queue_t *match_queue);

/*
 * dealloc_match_queue_between_rounds() - It frees the memory used by the match queue
                                          between rounds, as it is not done when using
                                          dequeue_match().
 * @arg1: Pointer to the match_queue.
 */
void dealloc_match_queue_between_rounds(match_queue_t *match_queue);

/*
 * stack_push() - Pushes a team node into the stack.
 * @arg1: Pointer to the stack's top address, so it can modify it, if nedded.
 * @arg2: Value to be pushed into the stack (pointer to a team node).
 */
void stack_push(stack_node_t **stack_top, team_node_t *value);

/*
 * stack_pop() - Pop the value from the stack (pointer to a team node).
 * @arg1: Pointer to the stack's top address, so it can modify it, if nedded.
 * Returns the popped value (pointer to team node).
 */
team_node_t *stack_pop(stack_node_t **stack_top);

/*
 * copy_players_list() - Copy player list from the current team node list to the
                         newly created top 8 team list's nodes.
 * @arg1: The current team node.
 * @arg2: Pointer to the new team node's address being created.
 */
void copy_players_list(team_node_t *team_node, team_node_t **new_team_node);

/*
 * create_ranking8() - Create a new team list consisting of copies of the top 8
                       teams.
 * @arg1: Pointer to the team list's head.
 * Returns pointer to the new top 8 teams list's head.
 */
team_node_t *create_ranking8(team_node_t *team_list_head);

/*
 * gaming_phase() - Simulates ongoing rounds of the lan party competition.
                    There are a power of 2 number of teams. At the end there
                    will be only one winner. It prints the battle to output file.
 * @arg1: Pointer to team list's head's address to modify it if nedeed.
 * @arg2: Output file to print the battle into.
 * Returns a pointer to the newly created top 8 teams list's head.
 */
team_node_t *gaming_phase(team_node_t **team_list_head, FILE *output_file);

/*
 * ongoing_round() - For each match, it decides a winner and a loser based on
                     their score. It updates the winner's score and pushes both
                     of them to their respective stacks(winner stack/loser stack).
 * @arg1: Pointer to the match queue so it can dequeue a match.
 * @arg2: Pointer to the winner's stack's top address, so it can modify it, if nedded.
 * @arg3: Pointer to the loser's stack's top address, so it can modify it, if nedded.
 * @arg4: Output file to print the match into.
 * @arg5: Round number to print it at the beggining of the round.
 */
void ongoing_round(match_queue_t *match_queue, stack_node_t **winners_stack_top,
                   stack_node_t **losers_stack_top, FILE *output_file, int round_no);

/*
 * enqueue_and_print_winners() - Pushes each winner from the winner's stack to
                                 print and enqueue them for the next round.
 * @arg1: Output file to print the winners into.
 * @arg2: Pointer to the winner's stack's top address, so it can modify it, if nedded.
 * @arg3: Pointer to the match queue so it can enqueue a match.
 * @arg4: Round number to print it at the beggining of the round.
 */
void enqueue_and_print_winners(FILE *output_file, stack_node_t **winners_stack_top,
                               match_queue_t *match_queue, int round_no);

/*
 * delete_loser_teams() - Pops loser teams from the loser's stack and deletes them
                          from the team list.
 * @arg1: Pointer to the loser's stack's top address, so it can modify it, if nedded.
 * @arg2: Pointer to the team list's head, so it can modify it, if nedded when
 *        removing teams.
 */
void delete_loser_teams(stack_node_t **losers_stack_top, team_node_t **team_list_head);

/*
 * new_tree_node() - Allocates memory for a new tree node. It doesn't matter
                     if it's an AVL tree or BST tree.
 * @arg1: Required data to initialize the tree node's value (pointer to team node).
 * Returns pointer to the newly created tree node.
 */
tree_node_t *new_tree_node(team_node_t *data);

/*
 * create_bst() - Traverse through the rank8 list and insert each team into the BST.
 * @arg1: Rank8 list's head to iterate through.
 * @arg2: Pointer to the binary search tree's root to modifiy it if nedeed.
 */
void create_bst(team_node_t *rank8_list_head, tree_node_t **bst_root);

/*
 * bst_insert() - Insert a team into the BST in descending order.
 * @arg1: Current tree node.
 * @arg2: Team node to insert.
 */
tree_node_t *bst_insert(tree_node_t *tree_node, team_node_t *team_node);

/*
 * in_order_traversal() - Traverse through the BST in order and print each node's
                          required information (stored team's name).
 * @arg1: The root of the BST.
 * @arg2: Output file to print into.
 */
void in_order_traversal(tree_node_t *tree_root, FILE *output_file);

/*
 * node_height() - Returns the height of given node.
 * @arg1: Tree node to calculate it's height.
 */
int node_height(tree_node_t *tree_node);

/*
 * max() - Returns maximum between two integers.
 * @arg1: First integer.
 * @arg2: Second integer.
 */
int max(int a, int b);

/*
 * left_rotation() - Perform a left rotation on the AVL tree rooted at 'z'.
 * @arg1: Tree node to perform rotation on.
 */
tree_node_t *left_rotation(tree_node_t *z);

/*
 * left_rotation() - Perform a right rotation on the AVL tree rooted at 'z'.
 * @arg1: Tree node to perform rotation on.
 */
tree_node_t *right_rotation(tree_node_t *z);

/*
 * left_rotation() - Perform a left-right rotation on the AVL tree rooted at 'z'.
 * @arg1: Tree node to perform rotation on.
 */
tree_node_t *left_right_rotation(tree_node_t *z);

/*
 * left_rotation() - Perform a right-left rotation on the AVL tree rooted at 'z'.
 * @arg1: Tree node to perform rotation on.
 */
tree_node_t *right_left_rotation(tree_node_t *z);

/*
 * get_balance- Get the balance factor of a tree node.
 * @arg1: Tree node to get the balance factor.
 * Returns the balance factor.
 */
int get_balance(tree_node_t *tree_node);

/*
 * in_order_extraction() - Each team pointer stored in the BST is extraced in order
                           and inserted into an array in that order.
 * @arg1: BST's root.
 * @arg2: Array of team pointers to store the extracted ones.
 */
void in_order_extraction(tree_node_t *root, team_node_t **arr);

/*
 * create_avl() - Create AVL from the BST's values, extracted in order.
 * @arg1: Pointer to AVL's root's address to modify it's value inside main().
 * @arg2: BST's root.
 */
void create_avl(tree_node_t **avl_root, tree_node_t *bst_root);

/*
 * avl_insert() - Insert a team into the AVL and store it in descending order.
 * @arg1: Initially AVL's root.
 * @arg2: Team node to store.
 * Returns pointer to the new AVL.
 */
tree_node_t *avl_insert(tree_node_t *tree_node, team_node_t *team_node);

/*
 * current_level_order_traversal() - Get nodes from a specific level and print them
                                     to output file.
 * @arg1: Output file to print into.
 * @arg2: Tree's root.
 * @arg3: Level to find and print nodes from.
 */
void current_level_order_traversal(FILE *output_file, tree_node_t *root, int level);

/*
 * process_requests() - Given an array of requests, will perform a set of actions if
                        the said request is true(1) or false(0).
 * @arg1: Array of requests.
 * @arg2: Pointer to the team list's head's address to modifiy it if needed.
 * @arg3: Pointer to the BST's root's address to modifiy it if needed.
 * @arg4: Pointer to the AVL's root's address to modifiy it if needed.
 * @arg5: Output file used for printing into.
 * @arg6: Input file used for getting teams information.
 * @arg7: Pointer to the rank8 list's head's address to modifiy it if needed.
 */
void process_requests(int *requests_array, team_node_t **team_list_head,
                      tree_node_t **bst_root, tree_node_t **avl_root, FILE *output_file,
                      FILE *teams_input, team_node_t **rank8_list_head);

/*
 * dealloc_player_list - Deallocates memory used by a player list.
 * @arg1: Player's list's head.
 */
void dealloc_player_list(player_node_t *head);

/*
 * dealloc_team_list - Deallocates memory used by a team list.
 * @arg1: Team's list's head.
 */
void dealloc_team_list(team_node_t *head);

/*
 * dealloc_tree - Deallocates memory used by a tree structure.
 * @arg1: Tree's root.
 */
void dealloc_tree(tree_node_t *root);

/*
 * deallocate_memory - Deallocates memory used by the program.
 * @arg1: Team's list's head.
 * @arg2: Rank8's list's head.
 * @arg3: BST's root.
 * @arg4: AVL's root.
 * @arg5: Input file to close it.
 * @arg6: Requests file to close
 * @arg7: Output file to close it.
 */
void deallocate_memory(team_node_t *team_list_head, team_node_t *rank8_list_head,
                       tree_node_t *bst_root, tree_node_t *avl_root, FILE *teams_input,
                       FILE *requests_input, FILE *output_file);