#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Jan 18 22:46:18 2021

@author: bytedance
"""

class win_struct:
    def __init__(self,row, col, player):
        self.row = row
        self.col = col
        self.player = player

data = [
"........",
"........",
"ox.....x",
"xo.....x",
"oxo...ox",
"oxxo..ox"]

row_len = len(data)
col_len = len(data[0])

for i in range(0, row_len):
    data[i] = list(data[i])


# 0: down; 1: right; 2: diag-left-down; 3: diag-right-down
def winning_check(data, direction, cur_char, row, col, count):        
    next_left_col = col_len-1 if col-1 < 0 else col-1 
    next_right_col = col+1 if col+1 < col_len else 0
    
    if data[row][col] == cur_char:
        count += 1
        if direction == 0:
            if row+1 < row_len:
                return winning_check(data, direction, cur_char, row+1, col, count)
        elif direction == 1:
            return winning_check(data, direction, cur_char, row, next_right_col, count)
        elif direction == 2:
            if row+1 < row_len:
                return winning_check(data, direction, cur_char, row+1, next_left_col, count)
        elif direction == 3:
            if row+1 < row_len:
                return winning_check(data, direction, cur_char, row+1, next_right_col, count)
    
    if count == 4:
        return win_struct(row, col, cur_char)
    
    return win_struct(-1, -1, '.')
                
def winning(data):
    winners = []
    len_winners = 0
    for i in range(0, row_len):
        for j in range(0, col_len):
            if data[i][j] != '.':
                for m in range(0, 4):
                    s1 = winning_check(data, m, data[i][j], i, j, 0)
                    if s1.player != '.':
                        winners.append(s1)
                        len_winners+=1
                        break
    if len_winners == 0:
        return '.'
    elif len_winners == 1:
        return winners[0].player
    elif len_winners == 2:
        return 'd'
    

def capi_winner(data, direction, player, row, col, count):
    data[row][col] = data[row][col].upper()
    if count == 4:
        return
    
    next_left_col = col_len-1 if col-1 < 0 else col-1 
    next_right_col = col+1 if col+1 < col_len else 0
    
    count += 1
    if direction == 0:
        if row >= 0:
            capi_winner(data, direction, player, row-1, col, count)
    elif direction == 1:
        capi_winner(data, direction, player, row, next_left_col, count)
    elif direction == 2:
        if row >= 0:
            capi_winner(data, direction, player, row-1, next_right_col, count)
    elif direction == 3:
        if row >= 0:
            capi_winner(data, direction, player, row-1, next_left_col, count)
    
def fall_board(data):
    
    for col in range(0, col_len):    
        for i in range(row_len-1, -1, -1):
            key = data[i][col]
            j = i+1
            while j<row_len and data[j][col] == '.':
                data[j-1][col] = data[j][col]
                j+=1
            data[j-1][col] = key
 
def print_board(data):
    for i in range(0, row_len):
        for j in range(0, col_len):
            print(data[i][j], end="")
        print()
    
capi_winner(data, 0, data[5][7], 5, 7, 0)
print_board(data)