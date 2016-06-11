function [M m] = maze(n, dens)
    showProgress = false;
 
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
 
    m = 2*n+3;
    
    DEST_DIST   = floor((m - 4) ./ 2);
    
    M = NotVISITED(ones(m));    
    
    M([1 2 (end - 1) end], :) = WALL;
    M(:, [1 2 (end - 1) end]) = WALL;
    
    i = 2 + randi(2);
    while (i < (m - 1))
        M(i, 3:end - 2) = WALL;
        inc = randi(100);
        
        t = 0;
        for j = 1:length(dens)
            t = t + dens(j);
            if (inc <= t)
                inc = j;
                break;
            end
        end
        
        i = i + inc;
    end
    
    i = 2 + randi(2);
    while (i < (m - 1))
        M(3:end - 2, i) = WALL;
        inc = randi(100);
        
        t = 0;
        for j = 1:length(dens)
            t = t + dens(j);
            if (inc <= t)
                inc = j;
                break;
            end
        end

        i = i + inc;
    end
 
    %this finds the start point for the maze stuff which does things
    for i = 3:m - 2
        for j = 3:m - 2
            if (M(i, j) == NotVISITED)
                currentCell = [i j];
                break;
            end
        end
        if (exist('currentCell'))
            break;
        end
    end
    
    if (~exist('currentCell'))    
        image(M - VISITED);
        axis equal off;
        return;
    end
    
    M(currentCell(1), currentCell(2)) = VISITED;
    S = currentCell;
 
    while (~isempty(S))
        unvisitedNeigbors = findNeighbours(M, currentCell, NotVISITED, WALL);
 
        if (~isempty(unvisitedNeigbors))
            next = unvisitedNeigbors(randi(size(unvisitedNeigbors, 1)), :);
            M = fill(M, currentCell, next, NoWALL);
 
            if (~isempty(findNeighbours(M, next, NotVISITED, WALL)))
                S = [S; next];
            end
 
            currentCell = next;
            M(currentCell(1), currentCell(2)) = VISITED;
        else
            currentCell = S(1, :);
            S = S(2:end, :);
        end
 
        if (showProgress)
            image(M-VISITED);
            axis equal off;
            drawnow;
            pause(.0001);
        end
    end
    
    source = randi([3, m-2], 1, 2);
    if (M(source(1), source(2)) == WALL)
       neigh = findStreet(M, source, NoWALL);
       source = neigh(randi(size(neigh, 1)), :);
    end
    
    candidates = [source + [DEST_DIST DEST_DIST]
                  source + [-DEST_DIST -DEST_DIST]
                  source + [-DEST_DIST DEST_DIST]
                  source + [DEST_DIST -DEST_DIST]];
              
    while 1
        index = randi(size(candidates, 1));
        candidate = candidates(index, :);
        if (isValid(candidate, M))
            destinations = findStreet(M, candidate, NoWALL);
            dest = destinations(randi(size(destinations, 1)), :);
            break;
        end
        
        candidates(index, :) = [];
    end
    
    M(source(1), source(2)) = ENTRANCE;
    
    if (~exist('dest'))
        return;
    end
    
    M(dest(1), dest(2)) = GOAL;
    
    image(M - VISITED);
    axis equal off;
end

function neighbours = findNeighbours(M, p, niceValue, continueValue)
    s = size(M);
    n = s(1); 
    m = s(2);
    
    neighbours = [];
    
    % Search left.
    q = p - [1 0];
    while (q(1) > 2)
        if (M(q(1), q(2)) == niceValue)
            neighbours = [neighbours; q];
            break;
        end  
        
        if (M(q(1), q(2)) ~= continueValue)
            break;
        end
        
        q = q - [1 0];
    end
    
    % Search right.
    q = p + [1 0];
    while (q(1) < n - 1)
        if (M(q(1), q(2)) == niceValue)
            neighbours = [neighbours; q];
            break;
        end
        
        if (M(q(1), q(2)) ~= continueValue)
            break;
        end
        
        q = q + [1 0];
    end
    
    % Search up.
    q = p - [0 1];
    while (q(2) > 2)
        if (M(q(1), q(2)) == niceValue)
            neighbours = [neighbours; q];
            break;
        end
        
        if (M(q(1), q(2)) ~= continueValue)
            break;
        end
        
        q = q - [0 1];
    end
    
    % Search down.
    q = p + [0 1];
    while (q(2) < m - 1)
        if (M(q(1), q(2)) == niceValue)
            neighbours = [neighbours; q];
            break;
        end
        
        if (M(q(1), q(2)) ~= continueValue)
            break;
        end
        
        q = q + [0 1];
    end
end

function neighbours = findStreet(M, p, street)
    s = size(M);
    n = s(1); 
    m = s(2);
    
    neighbours = [];
    
    % Search left.
    q = p - [1 0];
    while (q(1) > 2)
        if (M(q(1), q(2)) == street)
            neighbours = [neighbours; q];
            break;
        end  
        
        q = q - [1 0];
    end
    
    % Search right.
    q = p + [1 0];
    while (q(1) < n - 1)
        if (M(q(1), q(2)) == street)
            neighbours = [neighbours; q];
            break;
        end
        
        q = q + [1 0];
    end
    
    % Search up.
    q = p - [0 1];
    while (q(2) > 2)
        if (M(q(1), q(2)) == street)
            neighbours = [neighbours; q];
            break;
        end
        
        q = q - [0 1];
    end
    
    % Search down.
    q = p + [0 1];
    while (q(2) < m - 1)
        if (M(q(1), q(2)) == street)
            neighbours = [neighbours; q];
            break;
        end
        
        q = q + [0 1];
    end
end

function M = fill(M, p, q, VALUE)
    if (p(1) ~= q(1))
        a = min(p(1), q(1));
        b = max(p(1), q(1));
        
        M(a:b, p(2)) = VALUE;
    end
    
    if (p(2) ~= q(2))
        a = min(p(2), q(2));
        b = max(p(2), q(2));
        
        M(p(1), a:b) = VALUE;
    end
end

function r = isValid(p, M)
    s = size(M)
    n = s(1)
    m = s(2)
    r = false;
    if (p(1) > 2 && p(1) < n - 1 && p(2) > 2 && p(2) < m - 1)
        r = true;
    end
end
