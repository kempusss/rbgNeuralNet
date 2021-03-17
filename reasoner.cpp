#include "reasoner.hpp"


namespace reasoner{
    static const std::string cells_names[9] = {
        "rx0y0",
        "rx1y0",
        "rx2y0",
        "rx0y1",
        "rx1y1",
        "rx2y1",
        "rx0y2",
        "rx1y2",
        "rx2y2",
    };

    std::string board_cell_to_string(int cell_id){
        return cells_names[cell_id];
    }

    static const std::string pieces_names[3] = {
        "e",
        "o",
        "x",
    };

    std::string piece_to_string(int piece_id){
        return pieces_names[piece_id];
    }

    static const std::string variables_names[2] = {
        "oplayer",
        "xplayer",
    };

    std::string variable_to_string(int variable_id){
        return variables_names[variable_id];
    }

    int action_to_modifier_index(int action_index){
        switch(action_index){
            case 1:
                return 0;
            case 2:
                return 1;
            case 3:
                return 2;
            case 9:
                return 3;
            case 10:
                return 4;
            case 13:
                return 5;
            case 14:
                return 6;
            case 15:
                return 7;
            case 17:
                return 8;
            case 23:
                return 9;
            case 24:
                return 10;
            case 27:
                return 11;
            case 28:
                return 12;
            case 29:
                return 13;
            default:
                return -1;
        }
    }

    int modifier_to_action_index(int modifier_index){
        switch(modifier_index){
            case 0:
                return 1;
            case 1:
                return 2;
            case 2:
                return 3;
            case 3:
                return 9;
            case 4:
                return 10;
            case 5:
                return 13;
            case 6:
                return 14;
            case 7:
                return 15;
            case 8:
                return 17;
            case 9:
                return 23;
            case 10:
                return 24;
            case 11:
                return 27;
            case 12:
                return 28;
            case 13:
                return 29;
            default:
                return -1;
        }
    }

    static const int cell_neighbors[10][4] = {
        {0,0,0,0},
        {4,0,2,0},
        {5,1,3,0},
        {6,2,0,0},
        {7,0,5,1},
        {8,4,6,2},
        {9,5,0,3},
        {0,0,8,4},
        {0,7,9,5},
        {0,8,0,6},
    };

    int get_neighbor(int cell_id, int edge_id){
        return cell_neighbors[cell_id+1][edge_id]-1;
    }

    static const int bounds[2] = {
        100,
        100,
    };

    int get_bound(int variable_id){
        return bounds[variable_id];
    }

    static const int shift_table1[10] = {
        0,0,0,0,0,1,2,0,4,5,
    };

    static const int shift_table2[10] = {
        0,9,0,0,0,1,2,0,4,5,
    };

    static const int shift_table3[10] = {
        0,5,6,0,8,9,0,0,0,0,
    };

    static const int shift_table4[10] = {
        0,7,8,9,1,2,3,4,5,6,
    };

    static const int shift_table5[10] = {
        0,0,0,0,2,3,0,5,6,0,
    };

    static const int shift_table6[10] = {
        0,0,0,7,2,3,0,5,6,0,
    };

    static const int shift_table7[10] = {
        0,0,4,5,0,7,8,0,0,0,
    };

    static const int shift_table8[10] = {
        0,3,1,2,6,4,5,9,7,8,
    };

    bool action_representation::operator==(const action_representation& rhs) const{
        return index == rhs.index and cell == rhs.cell;
    }
    move::move(const move_representation& mr)
    : mr(mr){
    }
    bool move::operator==(const move& rhs) const{
        return mr == rhs.mr;
    }
    int game_state::get_current_cell(void)const{
        return current_cell-1;
    }

    int game_state::get_piece(int cell_id)const{
        return pieces[cell_id+1];
    }

    int game_state::get_variable_value(int variable_id)const{
        return variables[variable_id];
    }

    int game_state::get_current_player(void)const{
        return current_player;
    }

    int game_state::get_player_score(int player_id)const{
        return variables[player_id-1];
    }

    void game_state::apply_move(const move& m){
        for(const auto& el: m.mr){
            apply_action(el);
        }
    }

    void game_state::get_all_moves(resettable_bitarray_stack&, std::vector<move>& moves){
        moves.clear();
        move_representation mr;
        switch(current_state){
            case 0:
                get_all_moves_0_1(current_cell, mr, moves);
                break;
            case 1:
                get_all_moves_1_2(current_cell, mr, moves);
                get_all_moves_1_23(current_cell, mr, moves);
                break;
            case 3:
                get_all_moves_3_4(current_cell, mr, moves);
                break;
            case 4:
                get_all_moves_4_5(current_cell, mr, moves);
                break;
            case 5:
                get_all_moves_5_6(current_cell, mr, moves);
                break;
            case 6:
                get_all_moves_6_8(current_cell, mr, moves);
                get_all_moves_6_10(current_cell, mr, moves);
                break;
            case 11:
                get_all_moves_11_12(current_cell, mr, moves);
                break;
            case 13:
                get_all_moves_13_14(current_cell, mr, moves);
                break;
            case 14:
                get_all_moves_14_15(current_cell, mr, moves);
                break;
            case 15:
                get_all_moves_15_16(current_cell, mr, moves);
                break;
            case 16:
                get_all_moves_16_18(current_cell, mr, moves);
                get_all_moves_16_20(current_cell, mr, moves);
                break;
            case 17:
                get_all_moves_17_23(current_cell, mr, moves);
                get_all_moves_17_2(current_cell, mr, moves);
                break;
            case 21:
                get_all_moves_21_22(current_cell, mr, moves);
                break;
            default:
                break;
        }
    }

    bool game_state::apply_any_move(resettable_bitarray_stack&){
        switch(current_state){
            case 0:
                if(apply_any_move_0_1(current_cell)){
                    return true;
                }
                return false;
            case 1:
                if(apply_any_move_1_2(current_cell)){
                    return true;
                }
                if(apply_any_move_1_23(current_cell)){
                    return true;
                }
                return false;
            case 3:
                if(apply_any_move_3_4(current_cell)){
                    return true;
                }
                return false;
            case 4:
                if(apply_any_move_4_5(current_cell)){
                    return true;
                }
                return false;
            case 5:
                if(apply_any_move_5_6(current_cell)){
                    return true;
                }
                return false;
            case 6:
                if(apply_any_move_6_8(current_cell)){
                    return true;
                }
                if(apply_any_move_6_10(current_cell)){
                    return true;
                }
                return false;
            case 11:
                if(apply_any_move_11_12(current_cell)){
                    return true;
                }
                return false;
            case 13:
                if(apply_any_move_13_14(current_cell)){
                    return true;
                }
                return false;
            case 14:
                if(apply_any_move_14_15(current_cell)){
                    return true;
                }
                return false;
            case 15:
                if(apply_any_move_15_16(current_cell)){
                    return true;
                }
                return false;
            case 16:
                if(apply_any_move_16_18(current_cell)){
                    return true;
                }
                if(apply_any_move_16_20(current_cell)){
                    return true;
                }
                return false;
            case 17:
                if(apply_any_move_17_23(current_cell)){
                    return true;
                }
                if(apply_any_move_17_2(current_cell)){
                    return true;
                }
                return false;
            case 21:
                if(apply_any_move_21_22(current_cell)){
                    return true;
                }
                return false;
            default:
                return false;
        }
    }

    int game_state::get_monotonicity_class(void){
        switch(current_state){
            case 3:
                return 0;
            case 13:
                return 1;
            default:
                return -1;
        }
    }

    bool game_state::is_legal([[maybe_unused]] const move& m)const{
        switch(current_state){
            case 3:
                switch(pieces[m.mr.front().cell]){
                    case 0:
                        break;
                    default:
                        return false;
                }
                return true;
            case 13:
                switch(pieces[m.mr.front().cell]){
                    case 0:
                        break;
                    default:
                        return false;
                }
                return true;
            default:
                return false;
        }
    }
    void game_state::apply_action(const action_representation& action){
        switch(action.index){
            case 1:
                variables[1] = 50;
                break;
            case 2:
                variables[0] = 50;
                break;
            case 3:
                current_player = 2;
                current_cell = action.cell;
                current_state = 3;
                break;
            case 9:
                current_player = 0;
                current_cell = action.cell;
                current_state = 5;
                break;
            case 10:
                pieces[action.cell] = 2;
                break;
            case 17:
                current_player = 1;
                current_cell = action.cell;
                current_state = 13;
                break;
            case 13:
                variables[1] = 100;
                break;
            case 14:
                variables[0] = 0;
                break;
            case 15:
                current_player = 0;
                current_cell = action.cell;
                current_state = 11;
                break;
            case 23:
                current_player = 0;
                current_cell = action.cell;
                current_state = 15;
                break;
            case 24:
                pieces[action.cell] = 1;
                break;
            case 27:
                variables[0] = 100;
                break;
            case 28:
                variables[1] = 0;
                break;
            case 29:
                current_player = 0;
                current_cell = action.cell;
                current_state = 21;
                break;
            default:
                break;
        }
    }

    bool game_state::evaluate0(int cell){
        if(get_pattern_value0_0_2(cell)){
            return true;
        }
        if(get_pattern_value0_0_4(cell)){
            return true;
        }
        if(get_pattern_value0_0_6(cell)){
            return true;
        }
        if(get_pattern_value0_0_8(cell)){
            return true;
        }
        if(get_pattern_value0_0_10(cell)){
            return true;
        }
        if(get_pattern_value0_0_12(cell)){
            return true;
        }
        if(get_pattern_value0_0_14(cell)){
            return true;
        }
        if(get_pattern_value0_0_16(cell)){
            return true;
        }
        return false;
    }

    bool game_state::evaluate1(int cell){
        if(get_pattern_value1_0_2(cell)){
            return true;
        }
        if(get_pattern_value1_0_4(cell)){
            return true;
        }
        if(get_pattern_value1_0_6(cell)){
            return true;
        }
        if(get_pattern_value1_0_8(cell)){
            return true;
        }
        if(get_pattern_value1_0_10(cell)){
            return true;
        }
        if(get_pattern_value1_0_12(cell)){
            return true;
        }
        if(get_pattern_value1_0_14(cell)){
            return true;
        }
        if(get_pattern_value1_0_16(cell)){
            return true;
        }
        return false;
    }

    bool game_state::evaluate2(int cell){
        if(get_pattern_value2_0_2(cell)){
            return true;
        }
        if(get_pattern_value2_0_4(cell)){
            return true;
        }
        if(get_pattern_value2_0_6(cell)){
            return true;
        }
        if(get_pattern_value2_0_8(cell)){
            return true;
        }
        if(get_pattern_value2_0_10(cell)){
            return true;
        }
        if(get_pattern_value2_0_12(cell)){
            return true;
        }
        if(get_pattern_value2_0_14(cell)){
            return true;
        }
        if(get_pattern_value2_0_16(cell)){
            return true;
        }
        return false;
    }

    bool game_state::evaluate3(int cell){
        if(get_pattern_value3_0_2(cell)){
            return true;
        }
        if(get_pattern_value3_0_4(cell)){
            return true;
        }
        if(get_pattern_value3_0_6(cell)){
            return true;
        }
        if(get_pattern_value3_0_8(cell)){
            return true;
        }
        if(get_pattern_value3_0_10(cell)){
            return true;
        }
        if(get_pattern_value3_0_12(cell)){
            return true;
        }
        if(get_pattern_value3_0_14(cell)){
            return true;
        }
        if(get_pattern_value3_0_16(cell)){
            return true;
        }
        return false;
    }

    bool game_state::apply_any_move_0_1([[maybe_unused]] int cell){
        [[maybe_unused]] int variable_change0 = variables[1];
        variables[1] = 50;
        [[maybe_unused]] int variable_change1 = variables[0];
        variables[0] = 50;
        if(apply_any_move_1_2(cell)){
            return true;
        }
        if(apply_any_move_1_23(cell)){
            return true;
        }
        variables[0] = variable_change1;
        variables[1] = variable_change0;
        return false;
    }

    bool game_state::apply_any_move_1_2([[maybe_unused]] int cell){
        current_player = 2;
        current_state = 3;
        current_cell = cell;
        return true;
    }

    bool game_state::apply_any_move_1_23([[maybe_unused]] int cell){
        return false;
    }

    bool game_state::apply_any_move_3_4([[maybe_unused]] int cell){
        for(int el=1;el<10;++el){
            if(apply_any_move_4_5(el)){
                return true;
            }
        }
        return false;
    }

    bool game_state::apply_any_move_4_5([[maybe_unused]] int cell){
        switch(pieces[cell]){
            case 0:
                break;
            default:
                return false;
        }
        current_player = 0;
        current_state = 5;
        current_cell = cell;
        return true;
    }

    bool game_state::apply_any_move_5_6([[maybe_unused]] int cell){
        [[maybe_unused]] int board_change0_cell = cell;
        [[maybe_unused]] int board_change0_piece = pieces[cell];
        pieces[cell] = 2;
        if(apply_any_move_6_8(cell)){
            return true;
        }
        if(apply_any_move_6_10(cell)){
            return true;
        }
        pieces[board_change0_cell] = board_change0_piece;
        return false;
    }

    bool game_state::apply_any_move_6_8([[maybe_unused]] int cell){
        if(evaluate0(cell)){
            return false;
        }
        current_player = 1;
        current_state = 13;
        current_cell = cell;
        return true;
    }

    bool game_state::apply_any_move_6_10([[maybe_unused]] int cell){
        if(not evaluate1(cell)){
            return false;
        }
        [[maybe_unused]] int variable_change0 = variables[1];
        variables[1] = 100;
        [[maybe_unused]] int variable_change1 = variables[0];
        variables[0] = 0;
        current_player = 0;
        current_state = 11;
        current_cell = cell;
        return true;
    }

    bool game_state::apply_any_move_11_12([[maybe_unused]] int cell){
        return false;
        current_player = 1;
        current_state = 13;
        current_cell = cell;
        return true;
    }

    bool game_state::apply_any_move_13_14([[maybe_unused]] int cell){
        for(int el=1;el<10;++el){
            if(apply_any_move_14_15(el)){
                return true;
            }
        }
        return false;
    }

    bool game_state::apply_any_move_14_15([[maybe_unused]] int cell){
        switch(pieces[cell]){
            case 0:
                break;
            default:
                return false;
        }
        current_player = 0;
        current_state = 15;
        current_cell = cell;
        return true;
    }

    bool game_state::apply_any_move_15_16([[maybe_unused]] int cell){
        [[maybe_unused]] int board_change0_cell = cell;
        [[maybe_unused]] int board_change0_piece = pieces[cell];
        pieces[cell] = 1;
        if(apply_any_move_16_18(cell)){
            return true;
        }
        if(apply_any_move_16_20(cell)){
            return true;
        }
        pieces[board_change0_cell] = board_change0_piece;
        return false;
    }

    bool game_state::apply_any_move_16_18([[maybe_unused]] int cell){
        if(evaluate2(cell)){
            return false;
        }
        if(apply_any_move_17_23(cell)){
            return true;
        }
        if(apply_any_move_17_2(cell)){
            return true;
        }
        return false;
    }

    bool game_state::apply_any_move_16_20([[maybe_unused]] int cell){
        if(not evaluate3(cell)){
            return false;
        }
        [[maybe_unused]] int variable_change0 = variables[0];
        variables[0] = 100;
        [[maybe_unused]] int variable_change1 = variables[1];
        variables[1] = 0;
        current_player = 0;
        current_state = 21;
        current_cell = cell;
        return true;
    }

    bool game_state::apply_any_move_17_23([[maybe_unused]] int cell){
        return false;
    }

    bool game_state::apply_any_move_17_2([[maybe_unused]] int cell){
        current_player = 2;
        current_state = 3;
        current_cell = cell;
        return true;
    }

    bool game_state::apply_any_move_21_22([[maybe_unused]] int cell){
        return false;
        if(apply_any_move_17_23(cell)){
            return true;
        }
        if(apply_any_move_17_2(cell)){
            return true;
        }
        return false;
    }

    void game_state::get_all_moves_0_1([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<move>& moves){
        int variable_change0 = variables[1];
        const auto previous_changes_list = mr.size();
        mr.emplace_back(1,cell);
        variables[1] = 50;
        int variable_change1 = variables[0];
        mr.emplace_back(2,cell);
        variables[0] = 50;
        get_all_moves_1_2(cell, mr, moves);
        get_all_moves_1_23(cell, mr, moves);
        mr.resize(previous_changes_list);
        variables[0] = variable_change1;
        variables[1] = variable_change0;
        return;
    }

    void game_state::get_all_moves_1_2([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<move>& moves){
        const auto previous_changes_list = mr.size();
        mr.emplace_back(3,cell);
        moves.emplace_back(mr);
        mr.resize(previous_changes_list);
        return;
    }

    void game_state::get_all_moves_1_23([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<move>& moves){
        return;
    }

    void game_state::get_all_moves_3_4([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<move>& moves){
        for(int el=1;el<10;++el){
            get_all_moves_4_5(el, mr, moves);
        }
        return;
    }

    void game_state::get_all_moves_4_5([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<move>& moves){
        switch(pieces[cell]){
            case 0:
                break;
            default:
                return;
        }
        const auto previous_changes_list = mr.size();
        mr.emplace_back(9,cell);
        moves.emplace_back(mr);
        mr.resize(previous_changes_list);
        return;
    }

    void game_state::get_all_moves_5_6([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<move>& moves){
        int board_change0_cell = cell;
        int board_change0_piece = pieces[cell];
        const auto previous_changes_list = mr.size();
        mr.emplace_back(10,cell);
        pieces[cell] = 2;
        get_all_moves_6_8(cell, mr, moves);
        get_all_moves_6_10(cell, mr, moves);
        mr.resize(previous_changes_list);
        pieces[board_change0_cell] = board_change0_piece;
        return;
    }

    void game_state::get_all_moves_6_8([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<move>& moves){
        if(evaluate0(cell)){
            return;
        }
        const auto previous_changes_list = mr.size();
        mr.emplace_back(17,cell);
        moves.emplace_back(mr);
        mr.resize(previous_changes_list);
        return;
    }

    void game_state::get_all_moves_6_10([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<move>& moves){
        if(not evaluate1(cell)){
            return;
        }
        int variable_change0 = variables[1];
        const auto previous_changes_list = mr.size();
        mr.emplace_back(13,cell);
        variables[1] = 100;
        int variable_change1 = variables[0];
        mr.emplace_back(14,cell);
        variables[0] = 0;
        mr.emplace_back(15,cell);
        moves.emplace_back(mr);
        mr.resize(previous_changes_list);
        variables[0] = variable_change1;
        variables[1] = variable_change0;
        return;
    }

    void game_state::get_all_moves_11_12([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<move>& moves){
        return;
        const auto previous_changes_list = mr.size();
        mr.emplace_back(17,cell);
        moves.emplace_back(mr);
        mr.resize(previous_changes_list);
        return;
    }

    void game_state::get_all_moves_13_14([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<move>& moves){
        for(int el=1;el<10;++el){
            get_all_moves_14_15(el, mr, moves);
        }
        return;
    }

    void game_state::get_all_moves_14_15([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<move>& moves){
        switch(pieces[cell]){
            case 0:
                break;
            default:
                return;
        }
        const auto previous_changes_list = mr.size();
        mr.emplace_back(23,cell);
        moves.emplace_back(mr);
        mr.resize(previous_changes_list);
        return;
    }

    void game_state::get_all_moves_15_16([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<move>& moves){
        int board_change0_cell = cell;
        int board_change0_piece = pieces[cell];
        const auto previous_changes_list = mr.size();
        mr.emplace_back(24,cell);
        pieces[cell] = 1;
        get_all_moves_16_18(cell, mr, moves);
        get_all_moves_16_20(cell, mr, moves);
        mr.resize(previous_changes_list);
        pieces[board_change0_cell] = board_change0_piece;
        return;
    }

    void game_state::get_all_moves_16_18([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<move>& moves){
        if(evaluate2(cell)){
            return;
        }
        get_all_moves_17_23(cell, mr, moves);
        get_all_moves_17_2(cell, mr, moves);
        return;
    }

    void game_state::get_all_moves_16_20([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<move>& moves){
        if(not evaluate3(cell)){
            return;
        }
        int variable_change0 = variables[0];
        const auto previous_changes_list = mr.size();
        mr.emplace_back(27,cell);
        variables[0] = 100;
        int variable_change1 = variables[1];
        mr.emplace_back(28,cell);
        variables[1] = 0;
        mr.emplace_back(29,cell);
        moves.emplace_back(mr);
        mr.resize(previous_changes_list);
        variables[1] = variable_change1;
        variables[0] = variable_change0;
        return;
    }

    void game_state::get_all_moves_17_23([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<move>& moves){
        return;
    }

    void game_state::get_all_moves_17_2([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<move>& moves){
        const auto previous_changes_list = mr.size();
        mr.emplace_back(3,cell);
        moves.emplace_back(mr);
        mr.resize(previous_changes_list);
        return;
    }

    void game_state::get_all_moves_21_22([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<move>& moves){
        return;
        get_all_moves_17_23(cell, mr, moves);
        get_all_moves_17_2(cell, mr, moves);
        return;
    }

    bool game_state::get_pattern_value0_0_2([[maybe_unused]] int cell){
        cell = shift_table1[cell];
        switch(pieces[cell]){
            case 2:
                break;
            default:
                return false;
        }
        cell = shift_table2[cell];
        switch(pieces[cell]){
            case 2:
                break;
            default:
                return false;
        }
        return true;
    }

    bool game_state::get_pattern_value0_0_4([[maybe_unused]] int cell){
        cell = shift_table3[cell];
        switch(pieces[cell]){
            case 2:
                break;
            default:
                return false;
        }
        cell = shift_table3[cell];
        switch(pieces[cell]){
            case 2:
                break;
            default:
                return false;
        }
        return true;
    }

    bool game_state::get_pattern_value0_0_6([[maybe_unused]] int cell){
        cell = cell_neighbors[cell][3];
        switch(pieces[cell]){
            case 2:
                break;
            default:
                return false;
        }
        cell = shift_table4[cell];
        switch(pieces[cell]){
            case 2:
                break;
            default:
                return false;
        }
        return true;
    }

    bool game_state::get_pattern_value0_0_8([[maybe_unused]] int cell){
        cell = cell_neighbors[cell][0];
        switch(pieces[cell]){
            case 2:
                break;
            default:
                return false;
        }
        cell = cell_neighbors[cell][0];
        switch(pieces[cell]){
            case 2:
                break;
            default:
                return false;
        }
        return true;
    }

    bool game_state::get_pattern_value0_0_10([[maybe_unused]] int cell){
        cell = shift_table5[cell];
        switch(pieces[cell]){
            case 2:
                break;
            default:
                return false;
        }
        cell = shift_table6[cell];
        switch(pieces[cell]){
            case 2:
                break;
            default:
                return false;
        }
        return true;
    }

    bool game_state::get_pattern_value0_0_12([[maybe_unused]] int cell){
        cell = shift_table7[cell];
        switch(pieces[cell]){
            case 2:
                break;
            default:
                return false;
        }
        cell = shift_table7[cell];
        switch(pieces[cell]){
            case 2:
                break;
            default:
                return false;
        }
        return true;
    }

    bool game_state::get_pattern_value0_0_14([[maybe_unused]] int cell){
        cell = cell_neighbors[cell][1];
        switch(pieces[cell]){
            case 2:
                break;
            default:
                return false;
        }
        cell = shift_table8[cell];
        switch(pieces[cell]){
            case 2:
                break;
            default:
                return false;
        }
        return true;
    }

    bool game_state::get_pattern_value0_0_16([[maybe_unused]] int cell){
        cell = cell_neighbors[cell][2];
        switch(pieces[cell]){
            case 2:
                break;
            default:
                return false;
        }
        cell = cell_neighbors[cell][2];
        switch(pieces[cell]){
            case 2:
                break;
            default:
                return false;
        }
        return true;
    }

    bool game_state::get_pattern_value1_0_2([[maybe_unused]] int cell){
        cell = shift_table1[cell];
        switch(pieces[cell]){
            case 2:
                break;
            default:
                return false;
        }
        cell = shift_table2[cell];
        switch(pieces[cell]){
            case 2:
                break;
            default:
                return false;
        }
        return true;
    }

    bool game_state::get_pattern_value1_0_4([[maybe_unused]] int cell){
        cell = shift_table3[cell];
        switch(pieces[cell]){
            case 2:
                break;
            default:
                return false;
        }
        cell = shift_table3[cell];
        switch(pieces[cell]){
            case 2:
                break;
            default:
                return false;
        }
        return true;
    }

    bool game_state::get_pattern_value1_0_6([[maybe_unused]] int cell){
        cell = cell_neighbors[cell][3];
        switch(pieces[cell]){
            case 2:
                break;
            default:
                return false;
        }
        cell = shift_table4[cell];
        switch(pieces[cell]){
            case 2:
                break;
            default:
                return false;
        }
        return true;
    }

    bool game_state::get_pattern_value1_0_8([[maybe_unused]] int cell){
        cell = cell_neighbors[cell][0];
        switch(pieces[cell]){
            case 2:
                break;
            default:
                return false;
        }
        cell = cell_neighbors[cell][0];
        switch(pieces[cell]){
            case 2:
                break;
            default:
                return false;
        }
        return true;
    }

    bool game_state::get_pattern_value1_0_10([[maybe_unused]] int cell){
        cell = shift_table5[cell];
        switch(pieces[cell]){
            case 2:
                break;
            default:
                return false;
        }
        cell = shift_table6[cell];
        switch(pieces[cell]){
            case 2:
                break;
            default:
                return false;
        }
        return true;
    }

    bool game_state::get_pattern_value1_0_12([[maybe_unused]] int cell){
        cell = shift_table7[cell];
        switch(pieces[cell]){
            case 2:
                break;
            default:
                return false;
        }
        cell = shift_table7[cell];
        switch(pieces[cell]){
            case 2:
                break;
            default:
                return false;
        }
        return true;
    }

    bool game_state::get_pattern_value1_0_14([[maybe_unused]] int cell){
        cell = cell_neighbors[cell][1];
        switch(pieces[cell]){
            case 2:
                break;
            default:
                return false;
        }
        cell = shift_table8[cell];
        switch(pieces[cell]){
            case 2:
                break;
            default:
                return false;
        }
        return true;
    }

    bool game_state::get_pattern_value1_0_16([[maybe_unused]] int cell){
        cell = cell_neighbors[cell][2];
        switch(pieces[cell]){
            case 2:
                break;
            default:
                return false;
        }
        cell = cell_neighbors[cell][2];
        switch(pieces[cell]){
            case 2:
                break;
            default:
                return false;
        }
        return true;
    }

    bool game_state::get_pattern_value2_0_2([[maybe_unused]] int cell){
        cell = shift_table1[cell];
        switch(pieces[cell]){
            case 1:
                break;
            default:
                return false;
        }
        cell = shift_table2[cell];
        switch(pieces[cell]){
            case 1:
                break;
            default:
                return false;
        }
        return true;
    }

    bool game_state::get_pattern_value2_0_4([[maybe_unused]] int cell){
        cell = shift_table3[cell];
        switch(pieces[cell]){
            case 1:
                break;
            default:
                return false;
        }
        cell = shift_table3[cell];
        switch(pieces[cell]){
            case 1:
                break;
            default:
                return false;
        }
        return true;
    }

    bool game_state::get_pattern_value2_0_6([[maybe_unused]] int cell){
        cell = cell_neighbors[cell][3];
        switch(pieces[cell]){
            case 1:
                break;
            default:
                return false;
        }
        cell = shift_table4[cell];
        switch(pieces[cell]){
            case 1:
                break;
            default:
                return false;
        }
        return true;
    }

    bool game_state::get_pattern_value2_0_8([[maybe_unused]] int cell){
        cell = cell_neighbors[cell][0];
        switch(pieces[cell]){
            case 1:
                break;
            default:
                return false;
        }
        cell = cell_neighbors[cell][0];
        switch(pieces[cell]){
            case 1:
                break;
            default:
                return false;
        }
        return true;
    }

    bool game_state::get_pattern_value2_0_10([[maybe_unused]] int cell){
        cell = shift_table5[cell];
        switch(pieces[cell]){
            case 1:
                break;
            default:
                return false;
        }
        cell = shift_table6[cell];
        switch(pieces[cell]){
            case 1:
                break;
            default:
                return false;
        }
        return true;
    }

    bool game_state::get_pattern_value2_0_12([[maybe_unused]] int cell){
        cell = shift_table7[cell];
        switch(pieces[cell]){
            case 1:
                break;
            default:
                return false;
        }
        cell = shift_table7[cell];
        switch(pieces[cell]){
            case 1:
                break;
            default:
                return false;
        }
        return true;
    }

    bool game_state::get_pattern_value2_0_14([[maybe_unused]] int cell){
        cell = cell_neighbors[cell][1];
        switch(pieces[cell]){
            case 1:
                break;
            default:
                return false;
        }
        cell = shift_table8[cell];
        switch(pieces[cell]){
            case 1:
                break;
            default:
                return false;
        }
        return true;
    }

    bool game_state::get_pattern_value2_0_16([[maybe_unused]] int cell){
        cell = cell_neighbors[cell][2];
        switch(pieces[cell]){
            case 1:
                break;
            default:
                return false;
        }
        cell = cell_neighbors[cell][2];
        switch(pieces[cell]){
            case 1:
                break;
            default:
                return false;
        }
        return true;
    }

    bool game_state::get_pattern_value3_0_2([[maybe_unused]] int cell){
        cell = shift_table1[cell];
        switch(pieces[cell]){
            case 1:
                break;
            default:
                return false;
        }
        cell = shift_table2[cell];
        switch(pieces[cell]){
            case 1:
                break;
            default:
                return false;
        }
        return true;
    }

    bool game_state::get_pattern_value3_0_4([[maybe_unused]] int cell){
        cell = shift_table3[cell];
        switch(pieces[cell]){
            case 1:
                break;
            default:
                return false;
        }
        cell = shift_table3[cell];
        switch(pieces[cell]){
            case 1:
                break;
            default:
                return false;
        }
        return true;
    }

    bool game_state::get_pattern_value3_0_6([[maybe_unused]] int cell){
        cell = cell_neighbors[cell][3];
        switch(pieces[cell]){
            case 1:
                break;
            default:
                return false;
        }
        cell = shift_table4[cell];
        switch(pieces[cell]){
            case 1:
                break;
            default:
                return false;
        }
        return true;
    }

    bool game_state::get_pattern_value3_0_8([[maybe_unused]] int cell){
        cell = cell_neighbors[cell][0];
        switch(pieces[cell]){
            case 1:
                break;
            default:
                return false;
        }
        cell = cell_neighbors[cell][0];
        switch(pieces[cell]){
            case 1:
                break;
            default:
                return false;
        }
        return true;
    }

    bool game_state::get_pattern_value3_0_10([[maybe_unused]] int cell){
        cell = shift_table5[cell];
        switch(pieces[cell]){
            case 1:
                break;
            default:
                return false;
        }
        cell = shift_table6[cell];
        switch(pieces[cell]){
            case 1:
                break;
            default:
                return false;
        }
        return true;
    }

    bool game_state::get_pattern_value3_0_12([[maybe_unused]] int cell){
        cell = shift_table7[cell];
        switch(pieces[cell]){
            case 1:
                break;
            default:
                return false;
        }
        cell = shift_table7[cell];
        switch(pieces[cell]){
            case 1:
                break;
            default:
                return false;
        }
        return true;
    }

    bool game_state::get_pattern_value3_0_14([[maybe_unused]] int cell){
        cell = cell_neighbors[cell][1];
        switch(pieces[cell]){
            case 1:
                break;
            default:
                return false;
        }
        cell = shift_table8[cell];
        switch(pieces[cell]){
            case 1:
                break;
            default:
                return false;
        }
        return true;
    }

    bool game_state::get_pattern_value3_0_16([[maybe_unused]] int cell){
        cell = cell_neighbors[cell][2];
        switch(pieces[cell]){
            case 1:
                break;
            default:
                return false;
        }
        cell = cell_neighbors[cell][2];
        switch(pieces[cell]){
            case 1:
                break;
            default:
                return false;
        }
        return true;
    }

}
