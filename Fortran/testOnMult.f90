subroutine OnMult(m_ar, m_br)
    integer, intent(in) :: m_ar, m_br
    integer :: Time1, Time2, clock_rate, clock_max
    real*8, allocatable :: pha(:,:), phb(:,:), phc(:,:)
    real*8 :: temp, timing
    integer :: i, j, k, size
    integer :: unit

    allocate(pha(m_ar, m_ar), phb(m_ar, m_ar), phc(m_ar, m_ar))

    do i = 1, m_ar
        do j = 1, m_ar
            pha(i, j) = 1.0d0
        end do
    end do

    do i = 1, m_br
        do j = 1, m_br
            phb(i, j) = dble(i)
        end do
    end do

    call SYSTEM_CLOCK(count=Time1, count_rate=clock_rate, count_max=clock_max)

    do i = 1, m_ar
        do j = 1, m_br
            temp = 0.0d0
            do k = 1, m_ar
                temp = temp + pha(i, k) * phb(k, j)
            end do
            phc(i, j) = temp
        end do
    end do
    call SYSTEM_CLOCK(count=Time2)

    timing = real(Time2 - Time1) / real(clock_rate)

    ! Open CSV file for appending
    open(unit=unit, file='single_mult_fortran.csv', status='old', action='write', position='append', iostat=ierr)
    if (ierr /= 0) then
        print *, "Error opening file!"
        stop
    end if

    ! Write size and timing to CSV file
    if (unit == 10) then
        write(unit, '(A)') "size, time"
    end if
    write(unit, '(I0, A, F0.3)') m_ar, ",", timing

    ! Close CSV file
    close(unit)

    deallocate(pha, phb, phc)
end subroutine OnMult


program Main
    implicit none
    integer :: size, m_ar, m_br
    real*8 :: timing

    ! Initialize starting size
    size = 600

    ! Loop until size reaches 3000
    do while (size <= 3000)
        ! Set matrix dimensions
        m_ar = size
        m_br = size

        call OnMult(m_ar, m_br)

        call OnMult(m_ar, m_br)

        ! Increment size by 400
        size = size + 400
    end do

end program Main
