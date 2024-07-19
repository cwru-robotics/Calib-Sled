x_and_y = csvread("first_two_proc.csv");
z = csvread("last_proc.csv");

x_and_y_coords = x_and_y(:, 1:3);
z_coords = z(:, 1:3);

scatter3(x_and_y_coords(:, 1), x_and_y_coords(:, 2), x_and_y_coords(:, 3));
hold on;
axis equal;
scatter3(z_coords(:, 1), z_coords(:, 2), z_coords(:, 3));

pmean = mean(x_and_y_coords, 1);
R = bsxfun(@minus, x_and_y_coords, pmean);
[V, D] = eig(R' * R);
the_normal_vector = V(:, 1);
line([pmean(1) pmean(1)+the_normal_vector(1)], [pmean(2) pmean(2)+the_normal_vector(2)], [pmean(3) pmean(3)+the_normal_vector(3)]);
hold off;

the_z_vector = z_coords(end, :) - z_coords(1, :);
n = norm(the_z_vector);
the_z_vector = the_z_vector / n;

acos(dot(the_z_vector, the_normal_vector))