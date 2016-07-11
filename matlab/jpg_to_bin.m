input = 'image.jpg';
output = 'out_image.bin';
k = imread(input);
w = 640;
h = 480;
size(k)
vs = zeros([w*h, 1]);
for i = 1:w
    for j = 1:h
        r = uint32(k(j,i,1));
        g = uint32(k(j,i,2));
        b = uint32(k(j,i,3));
        v = 0;
        v = r * 2^24;
        v = v + g * 2^16;
        v = v + b * 2^8;
        uint32(v);
        vs((j-1)*w + i) = v;
    end
end
fid = fopen(output, 'w');
fwrite(fid, vs, 'uint32');
