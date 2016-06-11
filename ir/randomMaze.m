function M = randomMaze(n, progress)
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

    D           = [ up 
                    down 
                    left 
                    right];
    
    START       = [3 3];
    GOAL        = [m-2 m-2];
    CURRENT     = START;
    
    finished    = false;
   
    while (~finished)
        S = [];
        for  i = 1:4
            pos = CURRENT + D(i,:);
            if (isValid(pos, M, m))
                S = [S 
                     D(i,:)];
            end
        end
        
        len = size(S, 1);
        if (len > 1)
            index = randi(len);
            dir = S(index,:);
        else
            dir = S(1,:);
        end
        
        M(CURRENT(1), CURRENT(2)) = NoWALL;
        CURRENT = CURRENT + dir;
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
