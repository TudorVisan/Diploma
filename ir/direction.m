function angle = direction(p, dest)
    dx = p(1) - dest(1);
    dy = p(2) - dest(2);
    angle = atan2(dy, dx);
end
