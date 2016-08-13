package com.aaomidi.jchat.chat.engine;

import com.aaomidi.jchat.chat.Main;
import com.aaomidi.jchat.chat.handlers.ChatClientHandler;
import io.netty.bootstrap.Bootstrap;
import io.netty.buffer.ByteBuf;
import io.netty.channel.*;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.SocketChannel;
import io.netty.channel.socket.nio.NioSocketChannel;
import io.netty.util.CharsetUtil;

public class ChatClient {
    private final Main instance;
    EventLoopGroup workerGroup = new NioEventLoopGroup();
    private Channel channel;

    public ChatClient(Main instance) {
        this.instance = instance;
        try {
            this.create();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void create() throws Exception {
        workerGroup = new NioEventLoopGroup();
        Bootstrap b = new Bootstrap();
        b.group(workerGroup);
        b.channel(NioSocketChannel.class);
        b.option(ChannelOption.SO_KEEPALIVE, true);
        b.handler(new ChannelInitializer<SocketChannel>() {
            @Override
            public void initChannel(SocketChannel ch) throws Exception {
                ch.pipeline().addLast(new ChatClientHandler(instance));
            }
        });

        ChannelFuture f = b.connect("localhost", instance.getLoadedConfiguration().getPortNumber()).sync();

        channel = f.channel();

    }

    public void sendMessage(String message) {
        message += "\r\n";
        while (channel == null) {
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        ByteBuf buf = channel.alloc().buffer();
        buf.writeCharSequence(message, CharsetUtil.UTF_8);
        ChannelFuture future = channel.writeAndFlush(buf);
        try {
            future.await();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    public void shutdown() {
        workerGroup.shutdownGracefully();

    }
}
