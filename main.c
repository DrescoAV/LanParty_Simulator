#include "lanparty.h"

void process_requests(int *requests_array, team_node_t **team_list_head,
                      tree_node_t **bst_root, tree_node_t **avl_root, FILE *output_file,
                      FILE *teams_input, team_node_t **rank8_list_head)
{
    // If the 1st option is selected,  it will do task 1.
    if (requests_array[0] == 1)
    {
        *team_list_head = create_team_list(teams_input);
    }

    // If the 2nd option is selected,  it will do task 2.

    if (requests_array[1] == 1)
    {
        delete_lowest_points_teams(team_list_head);
    }

    // It doesn't matter if only one of the first two tasks are completed,
    // there will only be one display.
    display_request_1_2(output_file, *team_list_head);

    // If the 3rd option is 1, it will do task 3.
    if (requests_array[2] == 1)
    {
        *rank8_list_head = gaming_phase(team_list_head, output_file);
    }

    // If the 4th option is 1, it will do task 4.
    if (requests_array[3] == 1)
    {
        create_bst(*rank8_list_head, bst_root);
        fprintf(output_file, "\nTOP 8 TEAMS:\n");
        in_order_traversal(*bst_root, output_file);
    }

    // If the 5th option is 1, it will do task 5.
    if (requests_array[4] == 1)
    {
        create_avl(avl_root, *bst_root);
        fprintf(output_file, "\nTHE LEVEL 2 TEAMS ARE: \n");
        current_level_order_traversal(output_file, *avl_root, SECOND_LEVEL);
    }
}

int main(int argc, char **argv)
{
    // If the number of arguments (including the program's name) is different then 4,
    // it will close the program.
    if (argc != 4)
    {
        printf("Invalid number of arguments.");
        exit(1);
    }

    // Declaring and initialising files variables for input and output.
    FILE *requests_input = fopen(argv[1], "rt");
    DIE(requests_input == NULL, "Error opening requests_input file");
    FILE *teams_input = fopen(argv[2], "rt");
    DIE(teams_input == NULL, "Error opening teams_input file");
    FILE *output_file = fopen(argv[3], "wt");
    DIE(output_file == NULL, "Error opening output file");

    // Declaring and initialising with NULL variables that will be used in main.
    team_node_t *team_list_head = NULL;
    team_node_t *rank8_list_head = NULL;
    tree_node_t *bst_root = NULL, *avl_root = NULL;

    // Declaring an array of 5 elements that will be 1(execute task) or 0(don't execute task)
    // to select which tasks will be executed.
    int requests_array[5];
    int i;

    // Reading array of tasks from input file.
    for (i = 0; i < 5; i++)
        fscanf(requests_input, "%d", &requests_array[i]);

    // Processing each task based on the array of tasks.
    process_requests(requests_array, &team_list_head, &bst_root, &avl_root, output_file,
                     teams_input, &rank8_list_head);

    // Deallocating all dynamically allocated memory across the program.
    deallocate_memory(team_list_head, rank8_list_head, bst_root, avl_root,
                      teams_input, requests_input, output_file);

    return 0;
}