#ifndef REASONER
#define REASONER

#include <boost/container/static_vector.hpp>
#include <string>
#include <vector>

namespace reasoner{
    constexpr int BOARD_SIZE = 9;
    constexpr int NUMBER_OF_PLAYERS = 3;
    constexpr int NUMBER_OF_PIECES = 3;
    constexpr int NUMBER_OF_VARIABLES = 2;
    constexpr int BOARD_DEGREE = 4;
    constexpr int MONOTONIC_CLASSES = 2;

    std::string board_cell_to_string(int cell_id);
    std::string piece_to_string(int piece_id);
    std::string variable_to_string(int variable_id);
    constexpr int NUMBER_OF_MODIFIERS = 14;
    int action_to_modifier_index(int action_index);
    int modifier_to_action_index(int modifier_index);

    int get_neighbor(int cell_id, int edge_id);
    int get_bound(int variable_id);

    class resettable_bitarray_stack{};
    struct action_representation{
        int index;
        int cell;
        action_representation(void)=default;
        action_representation(int index,int cell)
        : index(index),
          cell(cell){
        }
        bool operator==(const action_representation& rhs) const;
    };

    typedef boost::container::static_vector<action_representation, 4> move_representation;
    struct move{
        move_representation mr;
        move(void) = default;
        move(const move_representation& mr);
        bool operator==(const move& rhs) const;
    };
    class game_state{
        public:
            int get_current_cell(void)const;
            int get_piece(int cell_id)const;
            int get_variable_value(int variable_id)const;
            int get_current_player(void)const;
            int get_player_score(int player_id)const;
            void apply_move(const move& m);
            void get_all_moves(resettable_bitarray_stack&, std::vector<move>& moves);
            bool apply_any_move(resettable_bitarray_stack&);
            int get_monotonicity_class(void);
            bool is_legal(const move& m)const;
        private:
            void apply_action(const action_representation& action);
            bool evaluate0(int cell);
            bool evaluate1(int cell);
            bool evaluate2(int cell);
            bool evaluate3(int cell);
            bool apply_any_move_0_1(int cell);
            bool apply_any_move_1_2(int cell);
            bool apply_any_move_1_23(int cell);
            bool apply_any_move_3_4(int cell);
            bool apply_any_move_4_5(int cell);
            bool apply_any_move_5_6(int cell);
            bool apply_any_move_6_8(int cell);
            bool apply_any_move_6_10(int cell);
            bool apply_any_move_11_12(int cell);
            bool apply_any_move_13_14(int cell);
            bool apply_any_move_14_15(int cell);
            bool apply_any_move_15_16(int cell);
            bool apply_any_move_16_18(int cell);
            bool apply_any_move_16_20(int cell);
            bool apply_any_move_17_23(int cell);
            bool apply_any_move_17_2(int cell);
            bool apply_any_move_21_22(int cell);
            void get_all_moves_0_1(int cell, move_representation& mr, std::vector<move>& moves);
            void get_all_moves_1_2(int cell, move_representation& mr, std::vector<move>& moves);
            void get_all_moves_1_23(int cell, move_representation& mr, std::vector<move>& moves);
            void get_all_moves_3_4(int cell, move_representation& mr, std::vector<move>& moves);
            void get_all_moves_4_5(int cell, move_representation& mr, std::vector<move>& moves);
            void get_all_moves_5_6(int cell, move_representation& mr, std::vector<move>& moves);
            void get_all_moves_6_8(int cell, move_representation& mr, std::vector<move>& moves);
            void get_all_moves_6_10(int cell, move_representation& mr, std::vector<move>& moves);
            void get_all_moves_11_12(int cell, move_representation& mr, std::vector<move>& moves);
            void get_all_moves_13_14(int cell, move_representation& mr, std::vector<move>& moves);
            void get_all_moves_14_15(int cell, move_representation& mr, std::vector<move>& moves);
            void get_all_moves_15_16(int cell, move_representation& mr, std::vector<move>& moves);
            void get_all_moves_16_18(int cell, move_representation& mr, std::vector<move>& moves);
            void get_all_moves_16_20(int cell, move_representation& mr, std::vector<move>& moves);
            void get_all_moves_17_23(int cell, move_representation& mr, std::vector<move>& moves);
            void get_all_moves_17_2(int cell, move_representation& mr, std::vector<move>& moves);
            void get_all_moves_21_22(int cell, move_representation& mr, std::vector<move>& moves);

            bool get_pattern_value0_0_2(int cell);
            bool get_pattern_value0_0_4(int cell);
            bool get_pattern_value0_0_6(int cell);
            bool get_pattern_value0_0_8(int cell);
            bool get_pattern_value0_0_10(int cell);
            bool get_pattern_value0_0_12(int cell);
            bool get_pattern_value0_0_14(int cell);
            bool get_pattern_value0_0_16(int cell);

            bool get_pattern_value1_0_2(int cell);
            bool get_pattern_value1_0_4(int cell);
            bool get_pattern_value1_0_6(int cell);
            bool get_pattern_value1_0_8(int cell);
            bool get_pattern_value1_0_10(int cell);
            bool get_pattern_value1_0_12(int cell);
            bool get_pattern_value1_0_14(int cell);
            bool get_pattern_value1_0_16(int cell);

            bool get_pattern_value2_0_2(int cell);
            bool get_pattern_value2_0_4(int cell);
            bool get_pattern_value2_0_6(int cell);
            bool get_pattern_value2_0_8(int cell);
            bool get_pattern_value2_0_10(int cell);
            bool get_pattern_value2_0_12(int cell);
            bool get_pattern_value2_0_14(int cell);
            bool get_pattern_value2_0_16(int cell);

            bool get_pattern_value3_0_2(int cell);
            bool get_pattern_value3_0_4(int cell);
            bool get_pattern_value3_0_6(int cell);
            bool get_pattern_value3_0_8(int cell);
            bool get_pattern_value3_0_10(int cell);
            bool get_pattern_value3_0_12(int cell);
            bool get_pattern_value3_0_14(int cell);
            bool get_pattern_value3_0_16(int cell);

            int current_cell = 1;
            int current_player = 0;
            int current_state = 0;
            int pieces[10] = {
                -1,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
            };
            int variables[2] = {};
    };

}

#endif
