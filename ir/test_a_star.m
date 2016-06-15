function steps = test_a_star()
    steps = [];
    
    for j = 1:4
        s = [];
        
        for i = 1:100
            disp(['Run ', num2str(j), ' iteration ', num2str(i)]);
            s = [s a_star(50, false, [80 10 5 2 1 1 1])];
        end
        
        steps = [steps; s];
    end
end