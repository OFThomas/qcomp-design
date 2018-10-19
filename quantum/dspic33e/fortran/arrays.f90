program arrays

implicit none

real, allocatable, dimension(:,:) :: a,b

allocate(a(2,2))
allocate(b(2,2))

a=0.0
b=0.0
a(1,1)=1.5
b(2,2)=4.0

write(*,*) mat_add(a,b)


contains 

    function mat_add(a,b)
    real, allocatable, dimension(:,:) ::mat_add, a, b
    mat_add = a + b
    end function mat_add 
end program arrays
