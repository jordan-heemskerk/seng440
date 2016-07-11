output = 'test_image.jpg';
input = 'out_image.bin';
w = 640;
h = 480;
fid = fopen(input, 'r');
vs = fread(fid, [w*h, 1], 'uint32');
im = uint8(zeros([h,w,3]));
for i = 1:w
    for j = 1:h
        v = vs((j-1)*w + i);
        r = uint8(mod(v / 2^24, 2^8));
        g = uint8(mod(v / 2^16, 2^8));
        b = uint8(mod(v / 2^8, 2^8));
        im(j,i,1) = r;
        im(j,i,2) = g;
        im(j,i,3) = b;
    end
end
imwrite(im, output, 'jpg', 'Quality', 100);
