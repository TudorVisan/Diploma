function M = keepRightMaze(n, progress)
%RANDOMMAZE Summary of this function goes here
%   Detailed explanation goes here

    [M, m] = maze(n);

    mymap = [1 1 1
            1 1 1
            0 0 0
            1 0 0
            0 1 0
            0 0 1];

    colormap(mymap);
    set(gcf,'color','w');

    ENTRANCE    = 2;
    NoWALL      = 0;
    WALL        = 1;
    GOAL        = 3;
    
    NotVISITED  = -1;
    VISITED     = -2;

    up          = [-1 0];
    down        = [1 0];
    left        = [0 -1];
    right       = [0 1];

    D           = [ down 
                    right 
                    up 
                    left];
    
    START       = [3 3];
    GOAL        = [m-2 m-2];
    CURRENT     = START;
    
    finished    = false;
    
    while (~finished)
        direction = D(1,:);
        myLeft = CURRENT + D(4,:);
        if (M(myLeft(1), myLeft(2)) == WALL) %left hand is on wall
            next = CURRENT + direction;
            if (~isValid(next, M, m)) %can't move forward
                %rotate right and check again
                [up, down, left, right] = rotateAxis(up, down, left, right, 1);
                D = [ down 
                      right 
                      up 
                      left];
                continue;
            end
        else
            %hand is no longer on the wall, rotate into the corridor
            [up, down, left, right] = rotateAxis(up, down, left, right, -1);
            D = [ down 
                  right 
                  up 
                  left];
            direction = D(1,:);
        end
        
        M(CURRENT(1), CURRENT(2)) = NoWALL;
        CURRENT = CURRENT + direction;
        M(CURRENT(1), CURRENT(2)) = ENTRANCE;
        
        if (CURRENT == GOAL)
            finished = true;
        end
        
        if (progress)
            image(M-VISITED);
            axis equal off;
            drawnow;
            pause(.02);
        end
    end
    
    image(M-VISITED);
    axis equal off;
end


function r = isValid(p, M, len)
    r = false;
    if (p(1) > 2 && p(1) < len && p(2) > 2 && p(2) < len && ~(M(p(1), p(2)) == 1))
        r = true;
    end
end


function [up, down, left, right] = rotateAxis(up, down, left, right, direction)
    if (direction == -1) %rotate to the left
        aux = left;
        left = up;
        up = right;
        right = down;
        down = aux;
    end
    
    if (direction == 1) %rotate to the right
        aux = right;
        right = up;
        up = left;
        left = down;
        down = aux;
    end
end
