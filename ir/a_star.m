function steps = a_star(n, display, dens)
    ENTRANCE    = 2;
    NoWALL      = 0;
    WALL        = 1;
    GOAL        = 3;

    NotVISITED  = -1;
    VISITED     = -2;
    
    PATH        = 4;

    PAUSE       = 0;

    mymap = [1 1 1
            1 1 1
            0 0 0
            1 0 0
            0 1 0
            0 0 1];

    colormap(mymap);
    set(gcf,'color','w');

    [M, ~, source, dest] = maze(n, dens);
    
    if display
        image(M-VISITED);
        axis equal off;
    end
    
%%%%%%%%%%%%%%%%%%%%%%%%% Actual maze solving %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % Blocks already visited.
    explored = [];

    % Discovered but not evaluated blocks.
    frontier = source;

    % Parent blocks of all blocks.
    parent = zeros(size(M));
    parent(:, :, 2) = zeros(size(M));

    % Cost of getting from source block to each block.
    g_cost = Inf(size(M));
    g_cost(source(1), source(2)) = 0;

    % Cost of getting from source block to dest block by passing through a
    % block.
    f_cost = Inf(size(M));
    f_cost(source(1), source(2)) = heuristic(source, source, dest);

    path = [];

    steps = 0;
    while size(frontier, 1) ~= 0
        steps = steps + 1;
        % Find best block to explore.
        best = Inf;
        for i = 1:size(frontier, 1)
            candidate = frontier(i, :);
            cost = f_cost(candidate(1), candidate(2));
            if cost <= best
                best = cost;
                i_best = i;
            end
        end

        current = frontier(i_best, :);
        frontier(i_best, :) = [];
        explored = [explored; current];

        % Draw exploration of maze.
        if ~isequal(current, source)
            M(current(1), current(2)) = PATH;

            if display
                image(M-VISITED);
                axis equal off;
                drawnow;
                pause(PAUSE);
            end
        end

        % Add neighbours to frontier.
        neighbour = [
            current - [1 0];
            current - [0 1];
            current + [1 0];
            current + [0 1];
            ];
        for i = 1:size(neighbour, 1)
            candidate = neighbour(i, :);
            if is_in_array(candidate, explored) || M(candidate(1), candidate(2)) == WALL
                continue;
            end
            
            if isequal(candidate, dest)
               return;
            end

            cost = g_cost(current(1), current(2)) + 1;
            if ~is_in_array(candidate, frontier)
               frontier = [frontier; candidate];
            elseif cost >= g_cost(candidate(1), candidate(2))
                continue;
            end

            parent(candidate(1), candidate(2), :) = current;
            g_cost(candidate(1), candidate(2)) = cost;
            f_cost(candidate(1), candidate(2)) = heuristic(current, candidate, dest);
        end
    end
%%%%%%%%%%%%%%%%%%%%%%%% /Actual maze solving %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    if display
        image(M-VISITED);
        axis equal off;
    end
end

function h = heuristic(S, A, B)
    a1 = direction(S, A);
    a2 = direction(A, B);
    h = abs(a2 - a1);
end

function found = is_in_array(element, array)
    found = false;

    for i = 1:size(array, 1)
        if isequal(array(i, :), element)
            found = true;
            break;
        end
    end
end
