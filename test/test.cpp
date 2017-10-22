#include <chrono>
#include <mutex>
#include <thread>
#include "../Tracy.hpp"
#include "../common/TracySystem.hpp"

void TestFunction()
{
    for(;;)
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
        ZoneScoped;
        std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
    }
}

void ResolutionCheck()
{
    for(;;)
    {
        {
            ZoneScoped;
            std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
        }
        {
            ZoneScoped;
            std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
        }
    }

}

void ScopeCheck()
{
    for(;;)
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
        ZoneScoped;
    }
}

static TracyLockable( std::mutex, mutex );

void Lock1()
{
    for(;;)
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( 4 ) );
        std::lock_guard<LockableBase( std::mutex )> lock( mutex );
        LockMark( mutex );
        ZoneScoped;
        std::this_thread::sleep_for( std::chrono::milliseconds( 4 ) );
    }
}

void Lock2()
{
    for(;;)
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( 3 ) );
        std::unique_lock<LockableBase( std::mutex )> lock( mutex );
        LockMark( mutex );
        ZoneScoped;
        std::this_thread::sleep_for( std::chrono::milliseconds( 5 ) );
    }
}

void Lock3()
{
    for(;;)
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
        std::unique_lock<LockableBase( std::mutex )> lock( mutex );
        LockMark( mutex );
        ZoneScoped;
        std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
    }
}

void Plot()
{
    unsigned char i = 0;
    for(;;)
    {
        for( int j=0; j<1024; j++ )
        {
            TracyPlot( "Test plot", (int64_t)i++ );
        }
        std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
    }
}

void MessageTest()
{
    for(;;)
    {
        TracyMessage( "Tock", 4 );
        std::this_thread::sleep_for( std::chrono::milliseconds( 5 ) );
    }
}

static int Fibonacci( int n )
{
    ZoneScoped;
    if( n < 2 ) return n;
    return Fibonacci( n-1 ) + Fibonacci( n-2 );
}

void DepthTest()
{
    for(;;)
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( 20 ) );
        Fibonacci( 15 );
    }
}

int main()
{
    auto t1 = std::thread( TestFunction );
    auto t2 = std::thread( TestFunction );
    auto t3 = std::thread( ResolutionCheck );
    auto t4 = std::thread( ScopeCheck );
    auto t5 = std::thread( Lock1 );
    auto t6 = std::thread( Lock2 );
    auto t7 = std::thread( Lock3 );
    auto t8 = std::thread( Plot );
    auto t9 = std::thread( Plot );
    auto t10 = std::thread( MessageTest );
    auto t11 = std::thread( DepthTest );

    tracy::SetThreadName( t1, "First thread" );
    tracy::SetThreadName( t2, "Second thread" );
    tracy::SetThreadName( t3, "Resolution check" );
    tracy::SetThreadName( t4, "Scope check" );
    tracy::SetThreadName( t5, "Lock 1" );
    tracy::SetThreadName( t6, "Lock 2" );
    tracy::SetThreadName( t7, "Lock 3" );
    tracy::SetThreadName( t8, "Plot 1" );
    tracy::SetThreadName( t9, "Plot 2" );
    tracy::SetThreadName( t10, "Message test" );
    tracy::SetThreadName( t11, "Depth test" );

    for(;;)
    {
        TracyMessageL( "Tick" );
        std::this_thread::sleep_for( std::chrono::milliseconds( 2 ) );
        {
            ZoneScoped;
            std::this_thread::sleep_for( std::chrono::milliseconds( 2 ) );
        }
        FrameMark;
    }
}
